#include "../include/control.h"

int main() {

    App app;

    init_app(&app);

    while (!app_should_close(&app)) {
        update(&app);
    }

    destroy_app(&app);

    return 0;
}
