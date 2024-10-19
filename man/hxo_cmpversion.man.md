% HXO_CMPVERSION(3) HXO Library Definitions
% bitwaree <bitware@duck.com>
% September 2024

# NAME
hxo_cmpversion -- compares two semantic versions.

# SYNOPSIS

```C
#include <hxo-user.h>              //Contains all the definitions needed to get started

int hxo_cmpversion(const char *current_version, const char *needed_version);
```

# DESCRIPTION

`hxo_cmpversion()` . The _current version_ string pointer is passed through the **first argument**, followed by the _target version_ string pointer.

- NOTE: Only the major(x), minor(y), and patch(z) is compared, and any suffix like `-beta`, `-rc0` will be ignored.

# RETURN VALUES

- If both versions match `hxo_cmpversion()` returns 0.
- If the current version is greater than the target version it returns 1.
- If the target/needed version is greater that the current version it returns -1.


# EXPANDS TO

```C
#include <stdio.h>

static inline int hxo_cmpversion(const char *current_version, const char *needed_version) {
    int v1_major, v1_minor, v1_patch;
    int v2_major, v2_minor, v2_patch;

    // Parse the version strings
    sscanf(current_version, "%d.%d.%d", &v1_major, &v1_minor, &v1_patch);
    sscanf(needed_version, "%d.%d.%d", &v2_major, &v2_minor, &v2_patch);

    // Compare major version
    if (v1_major > v2_major) return 1;
    if (v1_major < v2_major) return -1;

    // Compare minor version
    if (v1_minor > v2_minor) return 1;
    if (v1_minor < v2_minor) return -1;

    // Compare patch version
    if (v1_patch > v2_patch) return 1;
    if (v1_patch < v2_patch) return -1;

    // Versions are equal
    return 0;
}
```

# SEE ALSO
hxo-loader(1) -- general guide  
hxo-devl(3) -- hxo module development guide  
hxo_createthread(3) -- creates a thread.
