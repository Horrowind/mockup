#ifndef MOCKUP_LEVEL_HPP
#define MOCKUP_LEVEL_HPP

//#include <vector>
#include "cpu.hpp"
#include "tiles.hpp"

namespace Mockup {

    class Level {
    public:
        Level(const char* path, int levelnum);

        void load_level(int level);

        void renderLine(uint8_t* line, int linenum);
        uint8_t renderPixel(int x, int y);

        int getHeight();
        int getWidth();
        uint16_t getTile16(int x, int y);
        Tile8 getMap8(int i);
        Tile16 getMap16(int i);
        uint32_t* getPalette();
        void animate(unsigned char frame);

        
    private:
        const char* m_path;
        int m_levelnum;
        
        void load_map16();
        void load_map8();
        void load_palette();
        void load_objects();

        uint16_t* m_layer1 = NULL;
        uint16_t* m_layer2 = NULL;

        int m_height;
        int m_width;


        int m_screens;

        uint32_t m_palette[256];
        Tile8    m_map8[512];
        Tile16   m_map16[512];
        
        CPU m_cpu;
        
    };

}
#endif //MOCKUP_LEVEL_HPP
