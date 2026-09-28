## Firmware implementation quality — mandatory local checklist

Apply alongside the user's PRD and coding standard. Do not invent product requirements.

Before edits, inventory existing application entry points, driver APIs, source ownership by
build target, dependencies, initialization/interrupt ownership, linker regions and reserved
sections. Record implemented, partial and missing behavior. Preserve customer components;
justify necessary modifications. Never register sources already owned by a library directly
in the executable. Check actual ownership in the final link map, not just target names.

Before final handover, review the diff independently against the PRD, baseline and these
rules. Record issues and evidence, not just a generic "review passed". Run clean Debug and
Release builds when supported; check the final ELF/map for preserved linker/startup contracts.
Update documentation and requirement-to-code mappings. Keep implementation, build success and
hardware verification distinct. Never silently reclassify agreed hardware criteria as
host-only to obtain a green status.

Unit tests are not part of this work: they run at the end, from Artifacts -> Unit Tests. Do
not write test files, test runners or test plans.
