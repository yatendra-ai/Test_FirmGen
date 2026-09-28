# Host unit tests for this firmware

Run these implementation tests on this machine with a host C compiler and harness. Compile the real modules and mock hardware boundaries; a cross-compilation build is not a host test run.

## What the tests compile

The project's real sources, headers and build configuration, with the tests. Imported projects need not have a `firmware/` directory.

## What compiles, and what needs a mock

```
WHAT A HOST TEST COULD REACH IN THIS TREE (a file scan, no compiler needed):
  modules/ -- 2 source file(s), host-test candidates (compilation unverified)
  services/ -- 4 source file(s), host-test candidates (compilation unverified)
  utils/ -- 3 source file(s), host-test candidates (compilation unverified)
  app/ -- 1 source file(s), host-test candidates (compilation unverified)
```

This is a heuristic discovery scan, not compilation proof. SDK-dependent logic can need mocked boundaries; electrical behavior still requires a board.

## Suggested harness

CMake/CTest with Unity, or Ceedling with CMock where the headers listed above and the `*_io_t` callback structs the drivers take need mocks. The firmware is written for GCC; MSVC rejects extensions it uses, so prefer gcc or clang.

## The worklist (1 requirements)

`worklist.json` has the machine-readable version, including which files and symbols carry each requirement. Write ONE test per criterion.

### `FR-006` — Board Integration, LEDs & User Input

Each press of the onboard USER button shall toggle an application state.

- Each detected button-press event changes the application state from its current value to its toggled value. (expected: The state is toggled after each call and returns to its initial value after two calls.)

Code: `firmware/modules/button_logic.c`, `firmware/app/app.c`
Symbols: `button_logic_toggle_state`, `app_process_task`

## What to send back

A JSON file shaped like `results.example.json`. Four things in it are load-bearing:

- `host` and `harness` — evidence with no provenance is refused on import. Results from a stale tree look identical to fresh ones without them.
- `at` — when the tests ran, so a wide gap from the import time is visible.
- `sources_compiled` — how many FIRMWARE source files the test binaries compiled. If this is 0, the tests were the harness plus your own test files and asserted their own literals; that has happened here and was reported as coverage for five categories running. Report it honestly and it is recorded as the verdict it is.
- `tests[].name` — the exact test name. A claim of verification has to name a test from this list, so a name that does not match is rejected rather than assumed.

A test that could not be written is more useful reported than omitted: add it with `"outcome": "blocked"` and a `message` saying why. An absent test and a failing one are different facts, and only one of them is a problem with the firmware.
