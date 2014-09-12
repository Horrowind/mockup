#ifndef MOCKUP_MAIN_WINDOW_HPP
#define MOCKUP_MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QGraphicsView>

#include "testprocessor.hpp"

class TestCPU;

namespace Mockup {
    
    class MainWindow : public QMainWindow {
        Q_OBJECT
    public:
        MainWindow();
        ~MainWindow();
    
    private:
        void draw_normal_object(const char* path, uint8_t object, uint8_t size, uint8_t pos, uint8_t* subscreen_low_ptr, uint8_t* subscreen_high_ptr);
        void draw_extended_object(const char* path, uint8_t object, uint8_t pos, uint8_t* subscreen_low_ptr, uint8_t* subscreen_high_ptr);
        void load_map16(const char* path, int level, uint16_t* palette);
        
        
        QGraphicsView* view;
        QGraphicsScene scene;
        //TestCPU* cpu;
        
        uint8_t* layer1low = new uint8_t[27 * 16 * 0x14];
        uint8_t* layer1high = new uint8_t[27 * 16 * 0x14];
    };
}
#endif //MOCKUP_MAIN_WINDOW_HPP
