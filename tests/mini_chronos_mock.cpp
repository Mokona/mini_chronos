#include "mini_chronos_mock.h"

bool chrono_mock::now_was_called = false;
chrono_mock::duration chrono_mock::fake_duration{0};
