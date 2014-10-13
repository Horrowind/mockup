#include <cstdint>
#include <cstdio>
#include <cstring>

#include "addresses.hpp"
#include "misc.hpp"
#include "level.hpp"
#include "encryption.hpp"
#include "debug.hpp"

namespace Mockup {
    
    Level::Level(const char* path, int levelnum) : m_cpu(path) {
        m_path = path;
        load_level(levelnum);
    }

    Level::~Level() {
        if(m_layer1) delete m_layer1;
        if(m_layer2) delete m_layer2;
    }

    void Level::load_level(int levelnum) {
        m_levelnum = levelnum;
        m_cpu.init();

        load_palette();
        qDebug("Loaded Palette");
        load_map8();
        load_map16();
        load_objects();
        load_gfx3233();

        for(int i = 0; i < 8; i++) {
            animate(i);
        }
    }
    
    void Level::load_palette() {
        m_cpu.clear_ram();
        m_cpu.m_ram[0x65] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum]; 
        m_cpu.m_ram[0x66] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum + 1];
        m_cpu.m_ram[0x67] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum + 2];
        m_cpu.run(0x0583AC, 0x0583B8);
        // Run the palette loading routine.
        m_cpu.run(0x00ABED, 0x00ACEC);
        uint16_t paletteSNES[256];
        memcpy(paletteSNES, m_cpu.m_ram + 0x0703, 512);

        for(int i = 0; i < 256; i++) {
            m_palette[i] = i % 16 ? convertColor(paletteSNES[i]) : 0;
        }
    }

    void Level::load_map8() {

        m_cpu.clear_ram();
        m_cpu.m_ram[0x65] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum]; 
        m_cpu.m_ram[0x66] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum + 1];
        m_cpu.m_ram[0x67] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum + 2];
    
        m_cpu.run(0x0583AC, 0x0583B8);
        
        int tileset = m_cpu.m_ram[0x1931];
        auto address = 0x00292B + tileset * 4;
        int fg1 = m_cpu.m_rom[address + 0x00];
        int fg2 = m_cpu.m_rom[address + 0x01];
        int bg1 = m_cpu.m_rom[address + 0x02];
        int fg3 = m_cpu.m_rom[address + 0x03];

        uint8_t fg1Chr[3072], fg2Chr[3072], bg1Chr[3072], fg3Chr[3072];

        int addrFG1 = m_cpu.m_rom[originalGraphicsFilesBankByteTableLocationPC + fg1];
        addrFG1 = (addrFG1 << 8) | m_cpu.m_rom[originalGraphicsFilesHighByteTableLocationPC + fg1];
        addrFG1 = (addrFG1 << 8) | m_cpu.m_rom[originalGraphicsFilesLowByteTableLocationPC + fg1];
        int addrFG1PC = ((addrFG1 & 0x7f0000) >> 1) + (addrFG1 & 0x7fff);
        decryptLZ2(m_cpu.m_rom + addrFG1PC, fg1Chr);

        int addrFG2 = m_cpu.m_rom[originalGraphicsFilesBankByteTableLocationPC + fg2];
        addrFG2 = (addrFG2 << 8) | m_cpu.m_rom[originalGraphicsFilesHighByteTableLocationPC + fg2];
        addrFG2 = (addrFG2 << 8) | m_cpu.m_rom[originalGraphicsFilesLowByteTableLocationPC + fg2];
        int addrFG2PC = ((addrFG2 & 0x7f0000) >> 1) + (addrFG2 & 0x7fff);
        decryptLZ2(m_cpu.m_rom + addrFG2PC, fg2Chr);

        int addrBG1 = m_cpu.m_rom[originalGraphicsFilesBankByteTableLocationPC + bg1];
        addrBG1 = (addrBG1 << 8) | m_cpu.m_rom[originalGraphicsFilesHighByteTableLocationPC + bg1];
        addrBG1 = (addrBG1 << 8) | m_cpu.m_rom[originalGraphicsFilesLowByteTableLocationPC + bg1];
        int addrBG1PC = ((addrBG1 & 0x7f0000) >> 1) + (addrBG1 & 0x7fff);
        decryptLZ2(m_cpu.m_rom + addrBG1PC, bg1Chr);

        int addrFG3 = m_cpu.m_rom[originalGraphicsFilesBankByteTableLocationPC + fg3];
        addrFG3 = (addrFG3 << 8) | m_cpu.m_rom[originalGraphicsFilesHighByteTableLocationPC + fg3];
        addrFG3 = (addrFG3 << 8) | m_cpu.m_rom[originalGraphicsFilesLowByteTableLocationPC + fg3];
        int addrFG3PC = ((addrFG3 & 0x7f0000) >> 1) + (addrFG3 & 0x7fff);
        decryptLZ2(m_cpu.m_rom + addrFG3PC, fg3Chr);

        for(int tile = 0; tile < 512; tile++) {
            uint8_t* usedChr;
            switch(tile >> 7) {
            case 0b000:
                usedChr = fg1Chr;
                break;
            case 0b001:
                usedChr = fg2Chr;
                break;
            case 0b010:
                usedChr = bg1Chr;
                break;
            case 0b011:
                usedChr = fg3Chr;
                break;
            }

	    m_map8[tile] = Tile8::from3bpp(usedChr + 24 * (tile % 128));
        }
    }

    void Level::load_map16() {
        m_cpu.clear_ram();
        m_cpu.m_ram[0x65] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum]; 
        m_cpu.m_ram[0x66] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum + 1];
        m_cpu.m_ram[0x67] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum + 2];

        m_cpu.run(0x0583AC, 0x0583B8);

        int bgPalette = m_cpu.m_ram[0x1930];
              
        for(int i = 0; i < 512; i++) {
            int map16lookupSNES = 0x0D0000 + m_cpu.m_ram[2 * i + 0x0FBF] * 256 + m_cpu.m_ram[2 * i + 0x0FBE];
            int map16lookupPC   = ((map16lookupSNES & 0x7f0000) >> 1) + (map16lookupSNES & 0x7fff);
            
            Tile8   tiles[4];
            
            uint8_t palettes[4];
            for(int j = 0; j < 4; j++) {
                bool flipX  =  m_cpu.m_rom[map16lookupPC + j * 2 + 1] & 0b01000000;
                bool flipY  =  m_cpu.m_rom[map16lookupPC + j * 2 + 1] & 0b10000000;
                palettes[j] = (m_cpu.m_rom[map16lookupPC + j * 2 + 1] & 0b00011100) >> 2;
                int  tile   =  m_cpu.m_rom[map16lookupPC + j * 2] | ((m_cpu.m_rom[map16lookupPC + j * 2 + 1] & 0b00000011) << 8);
                tiles[j]    =  m_map8[tile].mirrored(flipX, flipY);
            }

            m_map16fg[i] = Tile16::fromTile8(tiles, palettes);

        }

        for(int i = 0; i < 512; i++) {
            int map16lookupSNES = 0x0D9100 + i*8;
            int map16lookupPC   = ((map16lookupSNES & 0x7f0000) >> 1) + (map16lookupSNES & 0x7fff);
            
            Tile8   tiles[4];
            
            uint8_t palettes[4];
            for(int j = 0; j < 4; j++) {
                bool flipX  =  m_cpu.m_rom[map16lookupPC + j * 2 + 1] & 0b01000000;
                bool flipY  =  m_cpu.m_rom[map16lookupPC + j * 2 + 1] & 0b10000000;
                palettes[j] =  (m_cpu.m_rom[map16lookupPC + j * 2 + 1] & 0b00011100) >> 2;
                int  tile   =  m_cpu.m_rom[map16lookupPC + j * 2] | ((m_cpu.m_rom[map16lookupPC + j * 2 + 1] & 0b00000011) << 8);
                tiles[j]    =  m_map8[tile].mirrored(flipX, flipY);
            }

            m_map16bg[i] = Tile16::fromTile8(tiles, palettes);

        }
    }

    void Level::load_gfx3233() {
        uint8_t gfx32Chr[23808];
        uint8_t gfx33Chr[12288];
        int addrGFX32PC = 0x40000;
        decryptLZ2(m_cpu.m_rom + addrGFX32PC, gfx32Chr);
        int addrGFX33PC = 0x43FC0;
        decryptLZ2(m_cpu.m_rom + addrGFX33PC, gfx33Chr);
  
        for(int i = 0; i < m_sizeOfGFX32; i++) {
            m_gfx3233[i] = Tile8::from4bpp(gfx32Chr + 32 * i);
        }
        for(int i = 0; i < m_sizeOfGFX33; i++) {
            m_gfx3233[i + m_sizeOfGFX32] = Tile8::from3bpp(gfx33Chr + 24 * i);
        }

    }


    void Level::load_objects() {
        m_cpu.clear_ram();
        
        for(int i = 0; i < 0x14 * 27 * 16; i++) {
            m_cpu.m_ram[0x0C800 + i] = 0x25;
            m_cpu.m_ram[0x1C800 + i] = 0;
        }

        m_cpu.m_ram[0x1933] = 0; //Object
        m_cpu.m_ram[0x65] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum]; 
        m_cpu.m_ram[0x66] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum + 1];
        m_cpu.m_ram[0x67] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum + 2];

        m_cpu.run(0x0583AC, 0x0583B8);

        
        int levelmode = m_cpu.m_ram[0x1925];
        int bgColorAddr = 0x30A0 + m_cpu.m_ram[0x192F];
        m_backgroundColor = convertColor(m_cpu.m_rom[bgColorAddr] | (m_cpu.m_rom[bgColorAddr + 1] << 8));
        switch(levelmode) {
        case 0x00:
        case 0x0C:
        case 0x0E:
        case 0x11:
        case 0x1E:
        case 0x0A:
        case 0x0D:
            m_hasLayer2Objects = true;
            break;
	    
        case 0x09: // \
        case 0x0B: // | Boss level
        case 0x10: // /
            m_layer1    = new uint16_t[1];
            m_layer1[0] = 0x25;
            m_width     = 1;
            m_height    = 1;
            m_hasLayer2Objects = false;
            return;
        default:
            m_hasLayer2Objects = false;
            break;
        }

        m_cpu.run(0x0583CF, 0x0583D2);
        int screens = m_cpu.m_ram[0x5D];
        bool isVerticalLevel = m_cpu.m_ram[0x5B] & 0x01;

        m_cpu.run(0x0583AC, 0x0583B8);
        m_hasLayer2BG = (m_cpu.m_rom[0x02E600 + 3 * m_levelnum + 2] == 0xFF);
        // m_cpu.m_ram[0x1933] = 1; //Object
        // m_cpu.run(0x0583CF, 0x0583D2);
    
        
        
    
        if(m_layer1) {
            delete[] m_layer1;
            m_layer1 = NULL;
        }
        if(m_layer2) {
            delete[] m_layer2;
            m_layer2 = NULL;
        }

        // Layer 1
        if(isVerticalLevel) {
            m_layer1 = new uint16_t[512 * screens];
            m_width = 32;
            m_height = screens * 16;
            for(int i = 0; i < 512 * screens; i++) {
                int xy = i % 256; int x = xy % 16; int y = xy >> 4;
                int sc = i >> 8;  int left = sc&1; int h = sc >> 1;
            
                int cx = left * 16 + x;
                int cy = h * 16 + y;
            
                m_layer1[cy * 32 + cx] = m_cpu.m_ram[0x1C800 + i] * 256 + m_cpu.m_ram[0x0C800 + i];
            }
        } else {
            m_layer1 = new uint16_t[432 * screens];
            m_width = screens * 16;
            m_height = 27;
            for(int i = 0; i < 432 * screens; i++) {
                int xy = i % 432;
                int sc = i / 432; 
                int cx = (xy % 16);
                int cy = xy >> 4;
                m_layer1[(cy * screens + sc) * 16 + cx] = m_cpu.m_ram[0x1C800 + i] * 256 + m_cpu.m_ram[0x0C800 + i];
            }
        }

        // Layer 2
        if(m_hasLayer2BG) {
            m_layer2 = new uint16_t[432*2];
            int addr = m_cpu.m_rom[0x02E600 + 3 * m_levelnum + 2];
            int offset = 0;
            if(addr == 0xFF) addr = 0x0C;
            addr = (addr << 8) | m_cpu.m_rom[0x02E600 + 3 * m_levelnum + 1];
            addr = (addr << 8) | m_cpu.m_rom[0x02E600 + 3 * m_levelnum + 0];
            if((addr & 0xFFFF) >= 0xE8FE) offset = 0x100;                              //See CODE_058046
            int pos = 0;
            while((m_cpu.op_read(addr) != 0xFF || m_cpu.op_read(addr + 1) != 0xFF) && pos < 432 * 2) {
                uint8_t cmd = m_cpu.op_read(addr);
                uint8_t length = cmd & 0b01111111;
                if(cmd & 0x80) {
                    for(int i = 0; i <= length; i++) m_layer2[pos + i] = offset + m_cpu.op_read(addr + 1);
                    pos += length + 1;
                    addr += 2;
                } else {
                    for(int i = 0; i <= length; i++) m_layer2[pos + i] = offset + m_cpu.op_read(addr + i + 1);
                    pos += length + 1;
                    addr += length + 2;
                }
            }
        } else if(m_hasLayer2Objects) {
            int addrLayer2LowTableEntryPC = m_cpu.m_rom[0x003DA8 + 2 * levelmode];
            int addrLayer2Low = m_cpu.m_rom[addrLayer2LowTableEntryPC] + (m_cpu.m_rom[addrLayer2LowTableEntryPC + 1] << 8);
            int addrLayer2HighTableEntryPC = m_cpu.m_rom[0x003DA8 + 2 * levelmode];
            int addrLayer2High = m_cpu.m_rom[addrLayer2LowTableEntryPC] + (m_cpu.m_rom[addrLayer2LowTableEntryPC + 1] << 8); 
            if(isVerticalLevel) {
                m_layer2 = new uint16_t[512 * screens];
                for(int i = 0; i < 512 * screens; i++) {
                    int xy = i % 256; int x = xy % 16; int y = xy >> 4;
                    int sc = i >> 8;  int left = sc&1; int h = sc >> 1;
            
                    int cx = left * 16 + x;
                    int cy = h * 16 + y;
            
                    m_layer2[cy * 32 + cx] = m_cpu.m_ram[addrLayer2Low + i] * 256 + m_cpu.m_ram[addrLayer2High + i];
                }
            } else {
                m_layer2 = new uint16_t[432 * screens];
                for(int i = 0; i < 432 * screens; i++) {
                    int xy = i % 432;
                    int sc = i / 432; 
                    int cx = (xy % 16);
                    int cy = xy >> 4;
                    m_layer2[(cy * screens + sc) * 16 + cx] = m_cpu.m_ram[addrLayer2Low + i] * 256 + m_cpu.m_ram[addrLayer2High + i];
                }
            }
        }
    }

    void Level::renderLineFG(uint8_t* line, int linenum) {
        int gy = linenum>>4;
        int by = linenum%16;
 
        for(int i = 0; i < m_width * 16; i++) {
            int pos = by * 16 + i % 16;
            line[i] = m_map16fg[m_layer1[gy * m_width + (i >> 4)]].pixels[pos];
        }
 
    }

    void Level::renderLineBG(uint8_t* line, int linenum) {
        if(m_hasLayer2BG) {
            int gy = (linenum>>4) % 27;
            int by = linenum%16;
            if(m_hasLayer2BG) {
                for(int i = 0; i < m_width * 16; i++) {
                    int pos = by * 16 + i % 16;
                    line[i] = m_map16bg[m_layer2[((i & 0x100) >> 4) * 27 + gy * 16 + ((i & 0x0FF) >> 4)]].pixels[pos];
                }
            } 
        } else if(m_hasLayer2Objects) {
            int gy = linenum>>4;
            int by = linenum%16;
            
            for(int i = 0; i < m_width * 16; i++) {
                int pos = by * 16 + i % 16;
                line[i] = m_map16bg[m_layer2[gy * m_width + (i >> 4)]].pixels[pos];
            }
        } else {
            for(int i = 0; i < m_width * 16; i++) line[i] = 0;
        }
    }

    //TODO: Method is not updated, see renderLine(F/B)G
    uint8_t Level::renderPixel(int x, int y) {
        int gx = x>>4;
        int bx = x%16;
        int gy = y>>4;
        int by = y%16;
        return m_map16fg[m_layer1[gy * m_width + gx]].pixels[by * 16 + bx];
    }

    int Level::getHeight() {
        return m_height;
    }

    int Level::getWidth() {
        return m_width;
    }

    bool Level::hasLayer2BG() {
        return m_hasLayer2BG;
    }

    bool Level::hasLayer2Objects() {
        return m_hasLayer2Objects;
    }

    bool Level::isVerticalLevel() {
        return m_isVerticalLevel;
    }

    uint32_t* Level::getPalette() {
        return m_palette;
    }

    uint32_t Level::getBackgroundColor() {
        return m_backgroundColor;
    }


    Tile8 Level::getMap8(int i) {
        return m_map8[i];
    }

    Tile8 Level::getGFX3233(int i) {
        return m_gfx3233[i];
    }


    Tile16 Level::getMap16fg(int i) {
        return m_map16fg[i];
    }

    Tile16 Level::getMap16bg(int i) {
        return m_map16bg[i];
    }

    uint16_t Level::getTile16(int x, int y) {
        return m_layer1[y * m_width + x];
    }


    void Level::animate(uint8_t frame) {
        //std::cout<<"Frame: " << (int)frame << std::endl;
        m_cpu.clear_ram();
        m_cpu.m_ram[0x14] = frame;
        m_cpu.run(0x00A418, 0x00A42F);
        int pos = m_cpu.op_read(0x2121);
        uint32_t snesColor = m_cpu.op_read(0x2122);
        m_cpu.step(); m_cpu.step();
        snesColor = snesColor | (m_cpu.op_read(0x2122) << 8);
        m_palette[pos] = convertColor(snesColor);

        m_cpu.clear_ram();
        m_cpu.m_ram[0x65] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum]; 
        m_cpu.m_ram[0x66] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum + 1];
        m_cpu.m_ram[0x67] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum + 2];
        m_cpu.run(0x0583AC, 0x0583B8);
        m_cpu.m_ram[0x0014] = frame;
        m_cpu.m_ram[0x14AD] = 0;
        m_cpu.m_ram[0x14AE] = 0;
        m_cpu.m_ram[0x14AF] = 0;
        m_cpu.regs.p = 0;
        m_cpu.regs.p.x = true;
        m_cpu.regs.p.m = true;
        //m_cpu.regs.d = 0x3000;

//        m_cpu.setDebug(true);
        m_cpu.run(0x00A5FD, 0x00A601);
//        m_cpu.setDebug(false);

        uint16_t source[3], dest[3];
        source[0] = ((m_cpu.m_ram[0x0D77] << 8) + m_cpu.m_ram[0x0D76] - 0x2000) / 32; 
        source[1] = ((m_cpu.m_ram[0x0D79] << 8) + m_cpu.m_ram[0x0D78] - 0x2000) / 32; 
        source[2] = ((m_cpu.m_ram[0x0D7B] << 8) + m_cpu.m_ram[0x0D7A] - 0x2000) / 32; 

        dest[0] = ((m_cpu.m_ram[0x0D7D] << 8) | m_cpu.m_ram[0x0D7C]) / 16;
        dest[1] = ((m_cpu.m_ram[0x0D7F] << 8) | m_cpu.m_ram[0x0D7E]) / 16; 
        dest[2] = ((m_cpu.m_ram[0x0D81] << 8) | m_cpu.m_ram[0x0D80]) / 16;
	       
        for(int i = 0; i < 3; i++) {
            if(dest[i] == 0 || ((source[i] & 0xFF00) == 0xFF00)) continue;
            if(dest[i] == 0x80) {                                              //See CODE_00A3DA
                m_map8[dest[i]] = m_gfx3233[source[i]];
                m_map8[dest[i] + 1] = m_gfx3233[source[i] + 1];
                m_map8[dest[i] + 16] = m_gfx3233[source[i] + 2];
                m_map8[dest[i] + 17] = m_gfx3233[source[i] + 3];
            } else {
                for(int j = 0; j < 4; j++) {
                    m_map8[dest[i] + j] = m_gfx3233[source[i] +j];
                }
            }
        }
        load_map16();
	
    }
}
