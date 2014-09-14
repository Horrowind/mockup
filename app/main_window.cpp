#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <QFileDialog>
#include <QInputDialog>
#include <QGraphicsItem>
#include <QKeyEvent>

#include "testprocessor.hpp"
#include "main_window.hpp"

#define WORLDLIB_IGNORE_DLL_FUNCTIONS
#include "WorldLib.hpp"


namespace Mockup {
    

    MainWindow::MainWindow() {
        filePath = QFileDialog::getOpenFileName(nullptr, "Open ROM", QString(), "*.smc");
        if(filePath == "") exit(0);
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
        
        std::vector<QPixmap> map8 = load_map8(path, level, cur_palette);
        
        for(int i = 0; i < 16; i++) {
            for(int j = 0; j < 32; j++) {
                 QGraphicsItem* item = scene.addPixmap(map8[j*16+i]);
                 item->setPos(8*i, 8*j);
            }
        }
          
        std::vector<QPixmap> map16 = load_map16(path, level);

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
                    if(!(layer1low[j * 16 * 27 + i * 16 + k] == 0x25 &&  layer1high[j * 16 * 27 + i * 16 + k] == 0)) {
                        //QGraphicsSimpleTextItem * item = scene.addSimpleText(QString::number(layer1low[j * 16 * 27 + i * 16 + k], 16));
                        //std::cout<<"Added: " << (int)layer1low[j * 16 * 27 + i * 16 + k] <<std::endl;
                        QGraphicsItem* item = scene.addPixmap(map16[ layer1high[j * 16 * 27 + i * 16 + k] * 256 + layer1low[j * 16 * 27 + i * 16 + k]]);
                        item->setPos((j*16+k)*16, i*16);
                        
                        QGraphicsSimpleTextItem * itemText = scene.addSimpleText(
                            QString::number(layer1low[j * 16 * 27 + i * 16 + k], 16),
                            QFont("Arial", -1, -1, (bool)layer1high[j * 16 * 27 + i * 16 + k]));
                        itemText->setPos((j*16+k)*16, i*16 + 2);
                                                
                        //std::cout << std::hex << (int)(layer1low[j * 16 * 27 + i * 16 + k] % 0x10);
                    } else {
                        //std::cout<<' ';
                    }
                }
            }
        }
        // for(int i = 0; i < 512; i++) {
        //     QGraphicsItem* item = scene.addPixmap(map16[i]);
        //     item->setPos(512 + (i % 16) * 16, (i >> 4) * 16);
        // }
        
        // for(int i = 0; i < 16; i++) {
        //     for(int j = 0; j < 16; j++) {
        //         uint16_t color = palette[16*i+j];
        //         int r = ((color & 0x1F) << 3);   r |= (r >> 5);
        //         int g = ((color & 0x3E0) >> 2);  g |= (g >> 5);
        //         int b = ((color & 0x7C00) >> 7); b |= (b >> 5);
        //         scene.addRect(j*8, i*8, 8, 8, QPen(), QBrush(QColor(r, g, b), Qt::SolidPattern));
        //     }
        // }

    }
 

    //void MainWindow::load_map8(const char* path, );



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
 

    std::vector<QPixmap> MainWindow::load_map8(const char* path, int level, int paletteNumber) {
        TestCPU cpu(path);
        cpu.m_object_low = NULL;
        cpu.m_object_high = NULL;
        uint16_t palette[256];

        cpu.m_ram[0x1933] = 0; //Object
        cpu.m_ram[0x65] = 0xDD; 
        cpu.m_ram[0x66] = 0x88;
        cpu.m_ram[0x67] = 0x06;

        cpu.set_cur_pos(0x0583B2);
        cpu.step();    
        while(cpu.filled_stack()) {
            cpu.step();
            //std::cout<<"Position1: " << std::hex<<cpu.get_cur_pos() <<std::endl;
        }
        cpu.step();
        //std::cout<<"Position2: " << std::hex<<cpu.get_cur_pos() << std::endl;
        while(cpu.filled_stack()) {
            cpu.step();
            //std::cout<<"Position3: " << std::hex<<cpu.get_cur_pos() << std::endl;
        }


        cpu.set_cur_pos(0x00A5BC);
        cpu.step();
        while(cpu.filled_stack()) {
             cpu.step();
        }
        memcpy(palette, cpu.m_ram + 0x0703, 512);

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

        std::vector<QPixmap> map8(512, QPixmap(8, 8));
        for(int tile = 0; tile < 512; tile++) {
            int tilemap = tile >> 7;
            //std::cout<< tilemap << std::endl;


            QImage img(8, 8, QImage::Format_ARGB32);
            for(int k = 0; k < 8; k++) {
                unsigned char bp1, bp2, bp3;
                switch(tilemap) {
                case 0b00:
                    bp1 = fg1Chr[24 * (tile % 128) + 2 * k];
                    bp2 = fg1Chr[24 * (tile % 128) + 2 * k + 1];
                    bp3 = fg1Chr[24 * (tile % 128) + 16 + k];
                    break;
                case 0b01:
                    bp1 = fg2Chr[24 * (tile % 128) + 2 * k];
                    bp2 = fg2Chr[24 * (tile % 128) + 2 * k + 1];
                    bp3 = fg2Chr[24 * (tile % 128) + 16 + k]; 
                    break;
                case 0b10:
                    bp1 = bg1Chr[24 * (tile % 128) + 2 * k];
                    bp2 = bg1Chr[24 * (tile % 128) + 2 * k + 1];
                    bp3 = bg1Chr[24 * (tile % 128) + 16 + k];
                    break;
                case 0b11:
                    bp1 = fg3Chr[24 * (tile % 128) + 2 * k];
                    bp2 = fg3Chr[24 * (tile % 128) + 2 * k + 1];
                    bp3 = fg3Chr[24 * (tile % 128) + 16 + k];
                    break;
                }

                for(int l = 0; l < 8; l++) {
                    int index = ((bp1 & (1 << (7 - l))) ? 1 : 0)
                        + ((bp2 & (1 << (7 - l))) ? 2 : 0)
                        + ((bp3 & (1 << (7 - l))) ? 4 : 0);
                    uint16_t colorSNES = palette[ 16 * paletteNumber + index];
                    int r = ((colorSNES & 0x1F) << 3);   r |= (r >> 5);
                    int g = ((colorSNES & 0x3E0) >> 2);  g |= (g >> 5);
                    int b = ((colorSNES & 0x7C00) >> 7); b |= (b >> 5);
                    int colorPC = index == 0 ? 0 : 0xFF000000 + (r << 16) + (g << 8) + b;
                    //std::cout << index << "," << std::hex << colorPC << ", ";
                    
                    img.setPixel(l, k, colorPC);
                }
            }
            map8[tile] = QPixmap::fromImage(img);
        }
        return map8;
    }
    std::vector<QPixmap> MainWindow::load_map16(const char* path, int level) {
        TestCPU cpu(path);
        cpu.m_object_low = NULL;
        cpu.m_object_high = NULL;
        uint16_t palette[256];

        cpu.m_ram[0x1933] = 0; //Object
        cpu.m_ram[0x65] = 0xDD; 
        cpu.m_ram[0x66] = 0x88;
        cpu.m_ram[0x67] = 0x06;

        cpu.set_cur_pos(0x0583B2);
        cpu.step();    
        while(cpu.filled_stack()) {
            cpu.step();
            //std::cout<<"Position1: " << std::hex<<cpu.get_cur_pos() <<std::endl;
        }
        cpu.step();
        //std::cout<<"Position2: " << std::hex<<cpu.get_cur_pos() << std::endl;
        while(cpu.filled_stack()) {
            cpu.step();
            //std::cout<<"Position3: " << std::hex<<cpu.get_cur_pos() << std::endl;
        }


        cpu.set_cur_pos(0x00A5BC);
        cpu.step();
        while(cpu.filled_stack()) {
             cpu.step();
        }
        memcpy(palette, cpu.m_ram + 0x0703, 512);

        std::ifstream in(path, std::ios::in | std::ios::binary);
        std::vector<unsigned char> rom;
        if (in) rom = std::vector<unsigned char>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
 
        auto romStart = rom.begin() + 0x200;
        auto romEnd = rom.end();

        std::vector<std::uint32_t> paletteWorldLib;
        worldlib::getLevelPalette(romStart, romEnd, std::back_inserter(paletteWorldLib), level);

        int fg1 = worldlib::getLevelSingleGraphicsSlot(romStart, romEnd, level, worldlib::GFXSlots::FG1);
        int fg2 = worldlib::getLevelSingleGraphicsSlot(romStart, romEnd, level, worldlib::GFXSlots::FG2);
        int bg1 = worldlib::getLevelSingleGraphicsSlot(romStart, romEnd, level, worldlib::GFXSlots::BG1);
        int fg3 = worldlib::getLevelSingleGraphicsSlot(romStart, romEnd, level, worldlib::GFXSlots::FG3);
        std::cout << std::hex << fg1 << ", " << fg2 << ", " << bg1 << ", " << fg3 << std::endl;
        std::vector<uint8_t> fg1Chr, fg2Chr, bg1Chr, fg3Chr;
        worldlib::decompressGraphicsFile(romStart, romEnd, std::back_inserter(fg1Chr), fg1);
        worldlib::decompressGraphicsFile(romStart, romEnd, std::back_inserter(fg2Chr), fg2);
        worldlib::decompressGraphicsFile(romStart, romEnd, std::back_inserter(bg1Chr), bg1);
        worldlib::decompressGraphicsFile(romStart, romEnd, std::back_inserter(fg3Chr), fg3);
        std::vector<QPixmap> map16(512, QPixmap(16, 16));
      
        for(int i = 0; i < 512; i++) {
            int map16lookupSNES = 0x0D0000 + cpu.m_ram[2 * i + 0x0FBF] * 256 + cpu.m_ram[2 * i + 0x0FBE];
            int map16lookupPC   = ((map16lookupSNES & 0x7f0000) >> 1) + (map16lookupSNES & 0x7fff) + 512;
            
            std::vector<QImage> tiles(4, QImage(8, 8, QImage::Format_ARGB32));
            //std::cout<<"Map16 Tile: " << std::hex << i;
            for(int j = 0; j < 4; j++) {
                tiles[j].fill(0xFFFFFFFF);
                
                bool flipX         =   rom[map16lookupPC + j * 2 + 1] & 0b01000000;
                bool flipY         =   rom[map16lookupPC + j * 2 + 1] & 0b10000000;
                int  paletteNumber =  (rom[map16lookupPC + j * 2 + 1] & 0b00011100) >> 2;
                int  tilemap       = ((rom[map16lookupPC + j * 2 + 1] & 0b00000011) << 1) + (rom[map16lookupPC + j * 2] >> 7);
                int  tile          =   rom[map16lookupPC + j * 2];
                //std::cout << "(" <<tilemap<< "," << tile << ") ";
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

                for(int k = 0; k < 8; k++) {
                    unsigned char bp1 = (*usedChr)[24 * (tile % 128) + 2 * k];
                    unsigned char bp2 = (*usedChr)[24 * (tile % 128) + 2 * k + 1];
                    unsigned char bp3 = (*usedChr)[24 * (tile % 128) + 16 + k];
                    for(int l = 0; l < 8; l++) {
                        int index = ((bp1 & (1 << (7 - l))) ? 1 : 0)
                            + ((bp2 & (1 << (7 - l))) ? 2 : 0)
                            + ((bp3 & (1 << (7 - l))) ? 4 : 0);
                        uint16_t colorSNES = palette[16 * paletteNumber + index];
                        int r = ((colorSNES & 0x1F) << 3);   r |= (r >> 5);
                        int g = ((colorSNES & 0x3E0) >> 2);  g |= (g >> 5);
                        int b = ((colorSNES & 0x7C00) >> 7); b |= (b >> 5);
                        //int colorPC = index == 0 ? 0 : 0xFF000000 + (r << 16) + (g << 8) + b;
                        int colorPC = paletteWorldLib[16 * paletteNumber + index];
                        //std::cout << index << "," << std::hex << colorPC << ", ";
                        tiles[j].setPixel(
                            flipX ? 7 - l : l,
                            flipY ? 7 - k : k,
                            colorPC);
                   }
                }
                //tiles.push_back(QImage(&bitmap[0], 8, 8, QImage::Format_ARGB32).copy(0, 0, 8, 8));
                //memcpy(tiles[j].scanline(0), &bitmap[0], 256);
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
