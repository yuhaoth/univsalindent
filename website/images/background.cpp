#include <iostream>

class FooFooBar : public FooBar
{
public:
    /* Init the foo */
    FooFooBar(int i);
private:
    // Give the foo a name
    char *oneFooBar = "TheOneFoo";
    FooBar getFooBar();
};

FooFooBar::FooFooBar(int i) {
    switch (i) {
        case 2 : 
            this->oneFooBar = "This is the real foo!";
            break;
        default:
            // Here end all foos
            break;
    }
}
