#include "mix_file.h"
#include "shp_ts_file.h"
#include "pal_file.h"
#include <filesystem>

int wmain(int argc, wchar_t* argv[])
{
    if (argc < 3) {
        wprintf(L"Usage: mixcli <mix> <pal>\n");
        return 1;
    }
    std::filesystem::path mixPath = argv[1];
    palette pal;  pal.load(argv[2]);

    mix_file mix; mix.open(mixPath.string().c_str());

    for (int i = 0; i < mix.get_c_files(); ++i) {
        std::string name = mix.get_name(i);
        mem_block buf;  mix.extract(i, buf);

        if (boost::iends_with(name, ".shp")) {
            shp_ts_file shp;  shp.open(buf);
            for (int f = 0; f < shp.get_c_frames(); ++f) {
                auto img = shp.decode_frame32(f, pal);
                std::string out = name + "_" + std::to_string(f) + ".png";
                image_file::write_png(out.c_str(), img);
            }
        }
        else {
            write_file(name, buf);
        }
    }
    return 0;
}
