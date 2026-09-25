# Host unit tests for this firmware

You are being asked to run unit tests that cannot run on the machine that generated this firmware -- that box carries the ESP-IDF cross-compilation toolchain and no host C compiler. Nothing here needs ESP-IDF.

## What to copy

The whole `firmware/` tree. Not a subset: a portable module includes headers from `interfaces/` and `configs/`, and those are not compiled but must be on the include path.

## What compiles, and what needs a mock

```
```

`platforms/` and `bsp/` are the vendor SDK's home and are not host-testable at all. Anything in them is checked on the board, not here.

## Suggested harness

Ceedling, with CMock for the headers listed above and for the `*_io_t` callback structs the drivers take. The firmware is written for GCC; MSVC rejects extensions it uses, so prefer gcc or clang.

## The worklist (0 requirements)

`worklist.json` has the machine-readable version, including which files and symbols carry each requirement. Write ONE test per criterion.

**Nothing is listed.** No requirement in this specification has a `host` acceptance criterion, so there is nothing a unit test can cover. That is a real answer, not an empty package -- do not invent tests to fill it. Every requirement here is checked on the board or by reading the code.

## What to send back

A JSON file shaped like `results.example.json`. Four things in it are load-bearing:

- `host` and `harness` — evidence with no provenance is refused on import. Results from a stale tree look identical to fresh ones without them.
- `at` — when the tests ran, so a wide gap from the import time is visible.
- `sources_compiled` — how many FIRMWARE source files the test binaries compiled. If this is 0, the tests were the harness plus your own test files and asserted their own literals; that has happened here and was reported as coverage for five categories running. Report it honestly and it is recorded as the verdict it is.
- `tests[].name` — the exact test name. A claim of verification has to name a test from this list, so a name that does not match is rejected rather than assumed.

A test that could not be written is more useful reported than omitted: add it with `"outcome": "blocked"` and a `message` saying why. An absent test and a failing one are different facts, and only one of them is a problem with the firmware.
