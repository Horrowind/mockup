#include <cstdint>
#include <cstdio>
#include <cstring>

#include <limits>
#include <fstream>
#include <iostream>
#include <vector>

#define WORLDLIB_IGNORE_DLL_FUNCTIONS
#include "WorldLib.hpp"

#include "misc.hpp"
#include "level.hpp"

namespace Mockup {
    
    Level::Level(const char* path, int levelnum) : m_cpu(path) {
        m_path = path;
        load_level(levelnum);
    }

    void Level::load_level(int levelnum) {
        m_levelnum = levelnum;
        
        load_palette();
	std::cout<<"palette"<<std::endl;
        load_map8();
	std::cout<<"map8"<<std::endl;
        load_map16();
	std::cout<<"map16"<<std::endl;
        load_objects();
	std::cout<<"objects"<<std::endl;
	animate(2);
    }

    void Level::load_palette() {
        m_cpu.clear_ram();
        m_cpu.m_ram[0x65] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum]; 
        m_cpu.m_ram[0x66] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum + 1];
        m_cpu.m_ram[0x67] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum + 2];
        
        // Run the palette loading routine.
        m_cpu.run(0x00ABED, 0x00ACEC);
        uint16_t paletteSNES[256];
        memcpy(paletteSNES, m_cpu.m_ram + 0x0703, 512);

        for(int i = 0; i < 256; i++) {
            m_palette[i] = i % 16 ? convertColor(paletteSNES[i]) : 0;
        }
    }

    void Level::load_map8() {

        std::ifstream in(m_path, std::ios::in | std::ios::binary);
        std::vector<unsigned char> rom;
        if (in) rom = std::vector<unsigned char>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
 
        auto romStart = rom.begin() + 0x200;
        auto romEnd = rom.end();



	// Table of the  low bytes of the locations of GFX00 - GFX31
	const int originalGraphicsFilesLowByteTableLocation = 0x00B992;
	// Table of the high bytes of the locations of GFX00 - GFX31
	const int originalGraphicsFilesHighByteTableLocation = 0x00B9C4;
	// Table of the bank bytes of the locations of GFX00 - GFX31
	const int originalGraphicsFilesBankByteTableLocation = 0x00B9F6;
	// List of the FG/BG slots levels can use (4 bytes each, 26 slots)
	const int backgroundSlotListTableLocation = 0x00A92B;
        
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

	//TODO: Remove traces of worldlib
        std::vector<uint8_t> fg1Chr, fg2Chr, bg1Chr, fg3Chr;
        worldlib::decompressGraphicsFile(romStart, romEnd, std::back_inserter(fg1Chr), fg1);
        worldlib::decompressGraphicsFile(romStart, romEnd, std::back_inserter(fg2Chr), fg2);
        worldlib::decompressGraphicsFile(romStart, romEnd, std::back_inserter(bg1Chr), bg1);
        worldlib::decompressGraphicsFile(romStart, romEnd, std::back_inserter(fg3Chr), fg3);

        for(int tile = 0; tile < 512; tile++) {
            std::vector<uint8_t>* usedChr;
            switch(tile >> 7) {
            case 0b000:
                usedChr = &fg1Chr;
                break;
            case 0b001:
                usedChr = &fg2Chr;
                break;
            case 0b010:
                usedChr = &bg1Chr;
                break;
            case 0b011:
                usedChr = &fg3Chr;
                break;
            }

            for(int k = 0; k < 8; k++) {
 
                unsigned char bp1, bp2, bp3;
                bp1 = usedChr->at(24 * (tile % 128) + 2 * k);
                bp2 = usedChr->at(24 * (tile % 128) + 2 * k + 1);
                bp3 = usedChr->at(24 * (tile % 128) + 16 + k);

                for(int l = 0; l < 8; l++) {
                    int index = ((bp1 & (1 << (7 - l))) ? 1 : 0)
                        + ((bp2 & (1 << (7 - l))) ? 2 : 0)
                        + ((bp3 & (1 << (7 - l))) ? 4 : 0);
                    
                    m_map8[tile].pixels[k * 8 + l] = index;
                }
            }
        }
    }

    void Level::load_map16() {
        m_cpu.clear_ram();
        m_cpu.m_ram[0x65] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum]; 
        m_cpu.m_ram[0x66] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum + 1];
        m_cpu.m_ram[0x67] = m_cpu.m_rom[0x02E000 + 3 * m_levelnum + 2];

        m_cpu.run(0x0583AC, 0x0583B8);
              
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

            m_map16[i] = Tile16::fromTile8(tiles, palettes);

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

	bool hasLayer2BG;
	int levelmode = m_cpu.m_ram[0x1925];
	switch(levelmode) {
	case 0x00:
	case 0x0C:
	case 0x0E:
	case 0x11:
	case 0x1E:
	case 0x0A:
	case 0x0D:
	    hasLayer2BG = false;
	    break;
	    
	case 0x09: // \
	case 0x0B: // | Boss level
	case 0x10: // /
	    m_layer1    = new uint16_t[1];
	    m_layer1[0] = 0x25;
            m_width     = 1;
            m_height    = 1;
	    return;
	default:
	    hasLayer2BG = true;
	}
        m_cpu.run(0x0583CF, 0x0583D2);
	
	m_cpu.run(0x0583AC, 0x0583B8);
	// m_cpu.m_ram[0x1933] = 1; //Object
        // m_cpu.run(0x0583CF, 0x0583D2);

        bool isVertical = m_cpu.m_ram[0x5B] & 0x01;
        int screens = m_cpu.m_ram[0x5D];// < 20 ? cpu.m_ram[0x5D] : 20;

        if(m_layer1) delete[] m_layer1;
        if(m_layer2) delete[] m_layer2;

        if(isVertical) {
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
            
	    //Layer 1
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

	    // Layer 2
	    if(!hasLayer2BG) {
		m_layer2 = new uint16_t[432*16];
		int addr = m_cpu.m_rom[0x02E600 + 3 * m_levelnum + 2];
		if(addr == 0xFF) addr = 0x0C;
		addr = (addr << 8) | m_cpu.m_rom[0x02E600 + 3 * m_levelnum + 1];
		addr = (addr << 8) | m_cpu.m_rom[0x02E600 + 3 * m_levelnum + 0];
		std::cout << std::hex << addr << std::endl;
		int pos = 0;
                while((m_cpu.op_read(addr) != 0xFF || m_cpu.op_read(addr + 1) != 0xFF) && pos < 432 * 2) {
		    uint8_t cmd = m_cpu.op_read(addr);
		    uint8_t length = cmd & 0b01111111;
		    if(cmd & 0x80) {
			for(int i = 0; i < length; i++) m_layer2[pos + i] = m_cpu.op_read(addr + 1);
			pos += length;
			addr += 2;
		    } else {
			for(int i = 0; i < length; i++) m_layer2[pos + i] = m_cpu.op_read(addr + i + 1);
			pos += length;
			addr += length + 1;
	      	    }
		}
		for(int i = 0; i < 27; i++) {
		    for(int k = 0; k < 2; k++) {
			for(int j = 0; j < 16; j++) {
			    std::cout.width(2);
			    std::cout << std::hex << (int)m_layer2[k *432+ i*16+j];
			}
		    }
		    std::cout << std::endl;
		}
	    }
	    
        }
    }

    void Level::renderLine(uint8_t* line, int linenum) {
        int gy = linenum>>4;
        int by = linenum%16;
        for(int i = 0; i < m_width * 16; i++) {
            int pos = by * 16 + i % 16;
            line[i] = m_map16[m_layer1[gy * m_width + (i >> 4)]].pixels[pos];
        }
    }

    uint8_t Level::renderPixel(int x, int y) {
        int gx = x>>4;
        int bx = x%16;
        int gy = y>>4;
        int by = y%16;
        return m_map16[m_layer1[gy * m_width + gx]].pixels[by * 16 + bx];
    }

    int Level::getHeight() {
        return m_height;
    }

    int Level::getWidth() {
        return m_width;
    }

    uint32_t* Level::getPalette() {
        return m_palette;
    }

    Tile8 Level::getMap8(int i) {
        return m_map8[i];
    }

    Tile16 Level::getMap16(int i) {
        return m_map16[i];
    }

    uint16_t Level::getTile16(int x, int y) {
        return m_layer1[y * m_width + x];
    }

    void Level::animate(unsigned char frame) {
        m_cpu.clear_ram();
        m_cpu.m_ram[0x14] = frame;
        m_cpu.run(0x00A418, 0x00A42F);
        int pos = m_cpu.op_read(0x2121);
        uint32_t snesColor = m_cpu.op_read(0x2122) << 8;
        m_cpu.step(); m_cpu.step();
        snesColor |= m_cpu.op_read(0x2122);
        m_palette[pos] = convertColor(snesColor);
        std::cout << std::hex << m_palette[pos] << std::endl;

    }
}
