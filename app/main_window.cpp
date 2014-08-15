#include <cstdint>
#include <cstdio>
#include <iostream>

#include <QFileDialog>
#include <QGraphicsItem>


#include "testprocessor.hpp"
#include "main_window.hpp"

namespace Mockup {
   
 
    MainWindow::MainWindow() {
        QString filePath = QFileDialog::getOpenFileName(this, "Open ROM", QString(), "*.smc");
        setDocumentMode(true);
        setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);
        view = new QGraphicsView(&scene);
        

        for(int i = 0; i < 0x14 * 27 * 16; i++) {
            layer1low[i] = 0x25;
            layer1high[i] = 0;
        }
        FILE* fp;
        fp = fopen(filePath.toLatin1().data(), "r");
        fseek(fp, 0x30add+5, SEEK_SET);
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
                         draw_extended_object(filePath.toLatin1().data(), output3, pos, layer1low + addr,  layer1high + addr);
                    }
                } else {
                    int output4 = fgetc(fp);
                }
            } else {
                int addr = 27 * 16 * screen + 256 * subscreen;
                uint8_t pos = (y << 4) | x;
                draw_normal_object(filePath.toLatin1().data(), obj, output3, pos, layer1low + addr,  layer1high + addr);
            }
        }
        load_map16(filePath.toLatin1().data(), 0x105);
        
        for(int i = 0; i < 27; i++) {
            for(int j = 0; j < 0x14; j++) {
                for(int k = 0; k < 16; k++) {
                    if(!(layer1low[j * 16 * 27 + i * 16 + k] == 0x25 &&  layer1high[j * 16 * 27 + i * 16 + k] == 0)) {
                        scene.addRect((j*16+k)*16, i*16, 16, 16);
                        QGraphicsSimpleTextItem * item = scene.addSimpleText(
                            QString::number(layer1low[j * 16 * 27 + i * 16 + k], 16));
                        item->setPos((j*16+k)*16, i*16+2);
                        //std::cout << std::hex << (int)(layer1low[j * 16 * 27 + i * 16 + k] % 0x10);
                    } else {
                        //std::cout<<' ';
                    }
                }
            }
        }
        MainWindow::setCentralWidget(view);
        
        
    }
    
    MainWindow::~MainWindow() {
        
    }
    
    //void MainWindow::load_map8(const char* path, );


 
    void MainWindow::load_map16(const char* path, int level) {
        TestCPU cpu(path);
        cpu.m_object_low = NULL;
        cpu.m_object_high = NULL;

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
        for(int i = 0x0FBE; i < 0x13BE; i += 2) {
            std::cout << "Map16:" << std::hex << cpu.m_ram[i+1]*256+cpu.m_ram[i] << std::endl;
        }

        cpu.set_cur_pos(0x00A5BC);
        cpu.step();
        while(cpu.filled_stack()) {
            cpu.step();
        }
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
