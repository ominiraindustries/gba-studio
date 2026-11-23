// PC driver of the DELVE generator for tools/gen_metrics.py (compiled with -DDL_HOST).
//   gen_host crc SEEDS LEVELS        v2 tables: "seed S: CRC CRC ..." like dungeon_model.py crc
//   gen_host dump SEEDS LEVELS OUT   game floors (v2 + hooks): binary records for the metrics
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dl_gen.h"

static dl::gen::level lv;
static uint8_t buf[8 * 1024 + 6 * 256];

int main(int argc, char** argv)
{
    if(argc < 4)
    {
        return 2;
    }

    int seeds = atoi(argv[2]);
    int levels = atoi(argv[3]);

    if(! strcmp(argv[1], "crc"))
    {
        for(int s = 0; s < seeds; ++s)
        {
            printf("seed %d:", s);

            for(int l = 0; l < levels; ++l)
            {
                if(! dl::gen::generate_v2(unsigned(s), l, dl::gen::v2_tables, lv))
                {
                    printf(" FAIL");
                    continue;
                }

                int n = dl::gen::level_bytes(lv, buf, int(sizeof(buf)));
                printf(" %04X", dl::gen::crc16(buf, n));
            }

            printf("\n");
        }

        return 0;
    }

    if(! strcmp(argv[1], "raw") && argc >= 5)      // v2 level_bytes of one (seed, level) for debugging
    {
        FILE* f = fopen(argv[4], "wb");
        dl::gen::generate_v2(unsigned(seeds), levels, dl::gen::v2_tables, lv);
        int n = dl::gen::level_bytes(lv, buf, int(sizeof(buf)));
        fwrite(buf, 1, size_t(n), f);
        fclose(f);
        printf("layout %d attempts %d entry %d stairs %d ncar %d\n", lv.layout, lv.attempts, lv.entry, lv.stairs, lv.ncar);
        return 0;
    }

    if(! strcmp(argv[1], "dump") && argc >= 5)
    {
        FILE* f = fopen(argv[4], "wb");

        if(! f)
        {
            return 3;
        }

        bool one = argc >= 6;      // dump SEED LEVEL OUT one: a single floor with that exact seed

        for(int s = one ? seeds : 0; s < (one ? seeds + 1 : seeds); ++s)
        {
            for(int l = one ? levels : 0; l < (one ? levels + 1 : levels); ++l)
            {
                unsigned used = dl::gen::generate_floor(unsigned(s), l, lv);
                int n = dl::gen::level_bytes(lv, buf, int(sizeof(buf)));
                int head[12] = { s, l, int(used), lv.layout, lv.entry, lv.entry_dir, lv.stairs, lv.attempts,
                                 lv.nhooks, lv.hook_removed_traps, lv.hook_changed_furn,
                                 dl::gen::solvable(lv, true) ? 1 : 0 };
                fwrite(head, sizeof(head), 1, f);
                fwrite(&n, sizeof(n), 1, f);
                fwrite(buf, 1, size_t(n), f);
                fwrite(lv.hooks, sizeof(dl::gen::hook), dl::gen::max_hooks, f);
                fwrite(lv.rooms, sizeof(dl::gen::room), dl::gen::max_rooms, f);
                fwrite(&lv.nrooms, sizeof(int), 1, f);
            }
        }

        fclose(f);
        return 0;
    }

    return 2;
}
