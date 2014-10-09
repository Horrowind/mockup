#ifndef MOCKUP_LEVEL_HPP
#define MOCKUP_LEVEL_HPP

//#include <vector>
#include "cpu.hpp"
#include "tiles.hpp"

namespace Mockup {

    class Level {
    public:
        Level(const char* path, int levelnum);

        void      load_level(int level);

        void      renderLineFG(uint8_t* line, int linenum);
        void      renderLineBG(uint8_t* line, int linenum);
        uint8_t   renderPixel(int x, int y);

        int       getHeight();
        int       getWidth();
        uint16_t  getTile16(int x, int y);
        Tile8     getMap8(int i);
        Tile16    getMap16fg(int i);
        Tile16    getMap16bg(int i);
	Tile8     getGFX3233(int i);

        uint32_t* getPalette();
        uint32_t  getBackgroundColor();

        void      animate(unsigned char frame);

        bool      hasLayer2BG();
        bool      hasLayer2Data();
        bool      isVerticalLevel();
        
    private:
        const char* m_path;
        int m_levelnum;
        
        void load_map16();
        void load_map8();
        void load_palette();
        void load_objects();
        void load_gfx3233();

        uint16_t* m_layer1 = NULL;
        uint16_t* m_layer2 = NULL;

        int m_height;
        int m_width;


        int m_screens;
        bool m_hasLayer2BG;
        bool m_hasLayer2Data;
        bool m_isVerticalLevel;
        uint32_t m_backgroundColor;
	

        uint32_t m_palette[256];
        Tile8    m_map8[512];
        Tile8    m_gfx3233[384];
        Tile16   m_map16fg[512];
        Tile16   m_map16bg[512];
        CPU m_cpu;
        
    };

}
#endif //MOCKUP_LEVEL_HPP
