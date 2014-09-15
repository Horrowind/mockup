#include <cstdint>
#include <cstdio>
#include <cstring>

#include <iostream>
#include <fstream>
#include <QFileDialog>
#include <QInputDialog>
#include <QGraphicsItem>
#include <QKeyEvent>


#define WORLDLIB_IGNORE_DLL_FUNCTIONS

#include "testprocessor.hpp"
#include "main_window.hpp"


#include "WorldLib.hpp"


namespace Mockup {
    

    MainWindow::MainWindow() {
        filePath = QFileDialog::getOpenFileName(nullptr, "Open ROM", QString(), "*.smc");
        if(filePath == "") exit(0);
        
TestCPU cpu(filePath.toLatin1().data(), true);
        cpu.set_cur_pos(0x0095B5);
        cpu.step();    
        while(cpu.filled_stack()) {
            cpu.step();
        }

        setDocumentMode(true);
        setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);
        view = new QGraphicsView(&scene);
        grabKeyboard();
        MainWindow::setCentralWidget(view);
        draw_level(filePath.toLatin1().data(), cur_level);

        
        

    }
    
    MainWindow::~MainWindow() {
        
    }
    
    void MainWindow::draw_level(const char* path, int level) {
        scene.clear();
        
        std::vector<uint32_t> palette = load_palette(path, level);
        //for(int i = 0; i < 256; i++) std::cout << std::hex << palette[i] << std::endl;
        
        std::vector<QImage> map8 = load_map8(path, level);
               
        std::vector<QPixmap> map16 = load_map16(path, level, map8, palette);

        for(int i = 0; i < 0x14 * 27 * 16; i++) {
            layer1low[i] = 0x25;
            layer1high[i] = 0;
        }
        FILE* fp;
        fp = fopen(path, "r");
        fseek(fp, 0x02E200 + 3 * level, SEEK_SET);
        unsigned int levelloc = getc(fp) | (getc(fp) << 8) | (getc(fp) << 16);
        std::cout << "Level location: " <<  std::hex << levelloc << std::endl;
        fseek(fp, ((levelloc & 0x7f0000) >> 1) + (levelloc & 0x7fff) + 517, SEEK_SET);
        uint8_t output, output2, output3;
        int screen = 0;
        while((output = getc(fp)) != 0xFF || output == -1) {
            output2 = getc(fp);
            output3 = getc(fp);
            if(output & 0x80) {
                screen++;               
            }
            uint8_t obj = ((output & 0x60) / 2) | (output2 / 16);
            uint8_t x = output2 & 0x0F;
            uint8_t y = output & 0x0F;
            uint8_t subscreen = !!(output & 0x10);
            if(obj == 0) {
                if(output3 != 0) {
                    int addr = 27 * 16 * screen + 256 * subscreen;
                    uint8_t pos = (y << 4) | x;
                    if(output3 == 1) {
                        screen = output;
                    } else {
                        draw_extended_object(path, output3, pos, layer1low + addr,  layer1high + addr);
                    }
                } else {
                    int output4 = fgetc(fp);
                }
            } else {
                int addr = 27 * 16 * screen + 256 * subscreen;
                uint8_t pos = (y << 4) | x;
                draw_normal_object(path, obj, output3, pos, layer1low + addr,  layer1high + addr);
            }
        }
	
        
        for(int i = 0; i < 27; i++) {
            for(int j = 0; j < 0x14; j++) {
                for(int k = 0; k < 16; k++) {
                    //QGraphicsSimpleTextItem * item = scene.addSimpleText(QString::number(layer1low[j * 16 * 27 + i * 16 + k], 16));
                    //std::cout<<"Added: " << (int)layer1low[j * 16 * 27 + i * 16 + k] <<std::endl;
                    QGraphicsItem* item = scene.addPixmap(map16[ layer1high[j * 16 * 27 + i * 16 + k] * 256 + layer1low[j * 16 * 27 + i * 16 + k]]);
                    item->setPos((j*16+k)*16, i*16);
                    
                    // QGraphicsSimpleTextItem * itemText = scene.addSimpleText(
                    //     QString::number(layer1low[j * 16 * 27 + i * 16 + k], 16),
                    //     QFont("Arial", -1, -1, (bool)layer1high[j * 16 * 27 + i * 16 + k]));
                    // itemText->setPos((j*16+k)*16, i*16 + 2);

                }
            }
        }
        for(int i = 0; i < 512; i++) {
            QGraphicsItem* item = scene.addPixmap(map16[i]);
            item->setPos(512 + (i % 16) * 16, (i >> 4) * 16);
        }
        
        // for(int i = 0; i < 16; i++) {
        //     for(int j = 0; j < 16; j++) {
        //         uint16_t color = palette[16*i+j];
        //         int r = ((color & 0x1F) << 3);   r |= (r >> 5);
        //         int g = ((color & 0x3E0) >> 2);  g |= (g >> 5);
        //         int b = ((color & 0x7C00) >> 7); b |= (b >> 5);
        //         scene.addRect(j*8, i*8, 8, 8, QPen(), QBrush(QColor(r, g, b), Qt::SolidPattern));
        //     }
        // }

        for(int i = 0; i < 16; i++) {
            for(int j = 0; j < 32; j++) {
                QImage img = map8[j*16+i];
                for(int k = 0; k < 8; k++) img.setColor(k, palette[16 * cur_palette + k]);
                QGraphicsItem* item = scene.addPixmap(QPixmap::fromImage(img));
                item->setPos(8*i, 8*j);
            }
        }

    }
 
    void MainWindow::keyPressEvent(QKeyEvent* event) {
        std::cout<<"huhu";
        if(event->key() == Qt::Key_PageUp && cur_level < 511) {
            cur_level++;
        } else if(event->key() == Qt::Key_PageDown && cur_level > 0) {
            cur_level--;
        } else if(event->key() == Qt::Key_Down && cur_palette > 0) {
            cur_palette--;
        } else if(event->key() == Qt::Key_Up && cur_palette < 0xF) {
            cur_palette++;
        } else {
            event->ignore();
            return;
        }
        event->accept();
        draw_level(filePath.toLatin1().data(), cur_level);
    }
 

    std::vector<uint32_t> MainWindow::load_palette(const char* path, int level) {
        //#define USEWORLDLIB
#ifdef USEWORLDLIB
        std::vector<uint32_t> palettePC;
        std::ifstream in(path, std::ios::in | std::ios::binary);
        std::vector<unsigned char> rom;
        if (in) rom = std::vector<unsigned char>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        
        auto romStart = rom.begin() + 0x200;
        auto romEnd = rom.end();
        worldlib::getLevelPalette(romStart, romEnd, std::back_inserter(palettePC), level);
#else
        std::vector<uint32_t> palettePC(256, 0);
        
        // Load CPU and specify, which level we want to open.
        TestCPU cpu(path);
        cpu.m_ram[0x65] = cpu.m_rom[0x02E200 + 3 * level]; 
        cpu.m_ram[0x66] = cpu.m_rom[0x02E200 + 3 * level + 1];
        cpu.m_ram[0x67] = cpu.m_rom[0x02E200 + 3 * level + 2];
        
        // Run the palette loading routine.
        cpu.set_cur_pos(0x00A5BC);
        cpu.step();
        while(cpu.filled_stack()) {
            cpu.step();
        }
        uint16_t paletteSNES[256];
        memcpy(paletteSNES, cpu.m_ram + 0x0703, 512);

        // Convert SNES colors to ARGB colors
        for(int i = 0; i < 256; i++) {
            int r = ((paletteSNES[i] & 0x1F) << 3);   r |= (r >> 5);
            int g = ((paletteSNES[i] & 0x3E0) >> 2);  g |= (g >> 5);
            int b = ((paletteSNES[i] & 0x7C00) >> 7); b |= (b >> 5);
            palettePC[i] = (i % 16)  ? (0xFF000000 + (r << 16) + (g << 8) + b) : 0;
        }
        return palettePC;
#endif

    }

    std::vector<QImage> MainWindow::load_map8(const char* path, int level) {
        TestCPU cpu(path);
        cpu.m_object_low = NULL;
        cpu.m_object_high = NULL;

        cpu.m_ram[0x1933] = 0; //Object
        cpu.m_ram[0x65] = cpu.m_rom[0x02E200 + 3 * level]; 
        cpu.m_ram[0x66] = cpu.m_rom[0x02E200 + 3 * level + 1];
        cpu.m_ram[0x67] = cpu.m_rom[0x02E200 + 3 * level + 2];

        cpu.set_cur_pos(0x0583B2);
        cpu.step();    
        while(cpu.filled_stack()) {
            cpu.step();
        }
        cpu.step();
        while(cpu.filled_stack()) {
            cpu.step();
        }

        std::ifstream in(path, std::ios::in | std::ios::binary);
        std::vector<unsigned char> rom;
        if (in) rom = std::vector<unsigned char>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
 
        auto romStart = rom.begin() + 0x200;
        auto romEnd = rom.end();

        int fg1 = worldlib::getLevelSingleGraphicsSlot(romStart, romEnd, level, worldlib::GFXSlots::FG1);
        int fg2 = worldlib::getLevelSingleGraphicsSlot(romStart, romEnd, level, worldlib::GFXSlots::FG2);
        int bg1 = worldlib::getLevelSingleGraphicsSlot(romStart, romEnd, level, worldlib::GFXSlots::BG1);
        int fg3 = worldlib::getLevelSingleGraphicsSlot(romStart, romEnd, level, worldlib::GFXSlots::FG3);
        std::vector<uint8_t> fg1Chr, fg2Chr, bg1Chr, fg3Chr;
        worldlib::decompressGraphicsFile(romStart, romEnd, std::back_inserter(fg1Chr), fg1);
        worldlib::decompressGraphicsFile(romStart, romEnd, std::back_inserter(fg2Chr), fg2);
        worldlib::decompressGraphicsFile(romStart, romEnd, std::back_inserter(bg1Chr), bg1);
        worldlib::decompressGraphicsFile(romStart, romEnd, std::back_inserter(fg3Chr), fg3);

        std::vector<QImage> map8(512, QImage(8, 8, QImage::Format_Indexed8));
        for(int tile = 0; tile < 512; tile++) {
            int tilemap = tile >> 7;
            std::vector<uint8_t>* usedChr;
            switch(tilemap) {
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

            for(int k = 0; k < 8; k++) map8[tile].setColor(k, 0xFF000000 + 0x222222 * k);
            for(int k = 0; k < 8; k++) {
 
                unsigned char bp1, bp2, bp3;
                bp1 = usedChr->at(24 * (tile % 128) + 2 * k);
                bp2 = usedChr->at(24 * (tile % 128) + 2 * k + 1);
                bp3 = usedChr->at(24 * (tile % 128) + 16 + k);

                for(int l = 0; l < 8; l++) {
                    int index = ((bp1 & (1 << (7 - l))) ? 1 : 0)
                        + ((bp2 & (1 << (7 - l))) ? 2 : 0)
                        + ((bp3 & (1 << (7 - l))) ? 4 : 0);
                    
                    map8[tile].setPixel(l, k, index);
                }
            }
        }
        return map8;
    }
    std::vector<QPixmap> MainWindow::load_map16(const char* path, int level, std::vector<QImage> map8, std::vector<uint32_t> palette) {
        TestCPU cpu(path);
        cpu.m_object_low = NULL;
        cpu.m_object_high = NULL;

        cpu.m_ram[0x1933] = 0; //We want to load Layer 1 Objects
        cpu.m_ram[0x65] = 0xDD; 
        cpu.m_ram[0x66] = 0x88;
        cpu.m_ram[0x67] = 0x06;

        cpu.set_cur_pos(0x0583B2);
        cpu.step();    
        while(cpu.filled_stack()) 
            cpu.step();

        cpu.step();
        while(cpu.filled_stack())
            cpu.step();
        


        // std::ifstream in(path, std::ios::in | std::ios::binary);
        // std::vector<unsigned char> rom;
        // if (in) rom = std::vector<unsigned char>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
 
        // auto romStart = rom.begin() + 0x200;
        // auto romEnd = rom.end();
        
        std::vector<QPixmap> map16(512, QPixmap(16, 16));
      
        for(int i = 0; i < 512; i++) {
            int map16lookupSNES = 0x0D0000 + cpu.m_ram[2 * i + 0x0FBF] * 256 + cpu.m_ram[2 * i + 0x0FBE];
            int map16lookupPC   = ((map16lookupSNES & 0x7f0000) >> 1) + (map16lookupSNES & 0x7fff);
            
            std::vector<QImage> tiles(4, QImage(8, 8, QImage::Format_ARGB32));
            //std::cout<<"Map16 Tile: " << std::hex << i;
            for(int j = 0; j < 4; j++) {
                tiles[j].fill(0xFFFFFFFF);
                
                bool flipX         =   cpu.m_rom[map16lookupPC + j * 2 + 1] & 0b01000000;
                bool flipY         =   cpu.m_rom[map16lookupPC + j * 2 + 1] & 0b10000000;
                int  paletteNumber =  (cpu.m_rom[map16lookupPC + j * 2 + 1] & 0b00011100) >> 2;
                int  tile          =   cpu.m_rom[map16lookupPC + j * 2] | ((cpu.m_rom[map16lookupPC + j * 2 + 1] & 0b00000011) << 8);
                
                tiles[j] = map8[tile].mirrored(flipX, flipY);
                for(int k = 0; k < 8; k++) tiles[j].setColor(k, palette[16 * paletteNumber + k]);
            }
            QPainter painter;
            painter.begin(&map16[i]);
            painter.fillRect(0,0,16,16, QColor::fromRgb(255, 255, 255));
            painter.drawImage(0, 0, tiles[0]);
            painter.drawImage(0, 8, tiles[1]);
            painter.drawImage(8, 0, tiles[2]);
            painter.drawImage(8, 8, tiles[3]); 
            painter.end();
        }
        return map16;
    }

    void MainWindow::draw_normal_object(const char* path, uint8_t object, uint8_t size, uint8_t pos, uint8_t* subscreen_low_ptr, uint8_t* subscreen_high_ptr) {
        TestCPU cpu(path);
        cpu.m_object_low = subscreen_low_ptr;
        cpu.m_object_high = subscreen_high_ptr;
        cpu.m_ram[0x5a] = object; //Object
        cpu.m_ram[0x59] = size; //Size
        cpu.m_ram[0x57] = pos;
        cpu.m_ram[0x6b] = 0x00; 
        cpu.m_ram[0x6c] = 0x20;
        cpu.m_ram[0x6d] = 0x00;
        cpu.m_ram[0x6e] = 0x00;
        cpu.m_ram[0x6f] = 0x50;
        cpu.m_ram[0x70] = 0x00;

        cpu.set_cur_pos(0x0DA40F);
        cpu.step();cpu.step();
    
        while(cpu.filled_stack()) {
            cpu.step();
        }
    }

    void  MainWindow::draw_extended_object(const char* path, uint8_t object, uint8_t pos, uint8* subscreen_low_ptr, uint8* subscreen_high_ptr) {
        TestCPU cpu(path);
        cpu.m_object_low = subscreen_low_ptr;
        cpu.m_object_high = subscreen_high_ptr;
//    cpu.m_ram[0x5a] = ; //Object
        cpu.m_ram[0x59] = object; //Object
        cpu.m_ram[0x57] = pos;
        cpu.m_ram[0x6b] = 0x00; 
        cpu.m_ram[0x6c] = 0x20;
        cpu.m_ram[0x6d] = 0x00;
        cpu.m_ram[0x6e] = 0x00;
        cpu.m_ram[0x6f] = 0x50;
        cpu.m_ram[0x70] = 0x00;

        cpu.set_cur_pos(0x0da100);
        cpu.step();cpu.step();
    
        while(cpu.filled_stack()) {
            cpu.step();
        }
    }


}
