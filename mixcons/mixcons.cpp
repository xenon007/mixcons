#include "mix_file.h"
#include "shp_ts_file.h"
#include "pal_file.h"
#include "palet.h"
#include "shp_decode.h"
#include "png_file.h"
#include <filesystem>
#include <cstring>
#include <boost/algorithm/string.hpp>

static Cvirtual_image decode_frame32(const Cshp_ts_file& shp, int frame, const t_palet pal)
{
    int global_cx = shp.cx();
    int global_cy = shp.cy();
    int cx = shp.get_cx(frame);
    int cy = shp.get_cy(frame);
    Cvirtual_binary frame_buf;
    if (shp.is_compressed(frame))
        decode3(shp.get_image(frame), frame_buf.write_start(cx * cy), cx, cy);
    else
        frame_buf = Cvirtual_binary(shp.get_image(frame), cx * cy);

    Cvirtual_binary canvas;
    std::memset(canvas.write_start(global_cx * global_cy), 0, global_cx * global_cy);
    byte* w = canvas.data_edit() + shp.get_x(frame) + global_cx * shp.get_y(frame);
    const byte* r = frame_buf.data();
    for (int y = 0; y < cy; ++y)
    {
        std::memcpy(w, r, cx);
        r += cx;
        w += global_cx;
    }

    Cvirtual_binary rgba;
    upsample_image(canvas.data(), reinterpret_cast<t_palet32entry*>(rgba.write_start(global_cx * global_cy * sizeof(t_palet32entry))), global_cx, global_cy, pal);
    return Cvirtual_image(rgba, global_cx, global_cy, 4, nullptr);
}

int wmain(int argc, wchar_t* argv[])
{
    if (argc < 3) {
        wprintf(L"Usage: mixcons <mix> <pal>\n");
        return 1;
    }

    std::filesystem::path mixPath = argv[1];
    Cpal_file pal_f; pal_f.open(argv[2]);
    t_palet pal; pal_f.decode(pal);

    Cmix_file mix; mix.open(mixPath.string().c_str());

    for (int i = 0; i < mix.get_c_files(); ++i) {
        int id = mix.get_id(i);
        std::string name = mix.get_name(id);
        Cvirtual_binary buf = mix.get_vdata(id);

        if (boost::iends_with(name, ".shp")) {
            Cshp_ts_file shp;  shp.load(buf);
            for (int f = 0; f < shp.cf(); ++f) {
                auto img = decode_frame32(shp, f, pal);
                std::string out = name + "_" + std::to_string(f) + ".png";
                img.save(out, ft_png);
            }
        } else {
            buf.save(name);
        }
    }
    return 0;
}