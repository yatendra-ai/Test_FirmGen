# Model preparation artifacts

This directory holds ST Edge AI model compiler output and reports. It is intentionally outside `firmware/` because generated workspaces may contain temporary C entry points and toolchain objects that must not be picked up by firmware source collection. The committed runtime sources and embedded generated model are registered as user-owned components in `firmgen_user_components.json`.
