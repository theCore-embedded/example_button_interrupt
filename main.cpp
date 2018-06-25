/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// For console
#include <ecl/iostream.hpp>
// For semaphore functionality
#include <ecl/thread/semaphore.hpp>
// For button class
#include <platform/exti_manager.hpp>
// For generated code
#include <aux/generated.hpp>

extern "C"
void board_init()
{
    gpio_init_generated();
}

static void button_interrupt_handler(void *ctx)
{
    ecl::semaphore *s = reinterpret_cast<ecl::semaphore*>(ctx);
    s->signal();
}

int main()
{
    ecl::cout << "Starting EXTI (button interrupt) demo" << ecl::endl;

    ecl::semaphore s;
    ecl::exti_manager::handler h;

    h.set_ctx(&s);
    h.set_cb(button_interrupt_handler);

    ecl::exti_manager::subscribe<ecl::button>(h, ecl::exti_manager::trigger::rising);

    while (1) {
        ecl::exti_manager::unmask(h);
        s.wait();
        ecl::cout << "Button pressed!" << ecl::endl;
    }
}
