# fs_usage_ng

This is my attempt to make the `fs_usage` tool from Apple work better
for my filesystem research.


### New features:
- the project compiles without private Apple headers
- compiles and runs on iOS (tested on iPhone SRD)
- displays both of `rename()`s parameters (extendable to other syscalls)
- can filter for only uid 0 processes ("-u" parameter)
- added handling of the missing `open()` flags:
  - `O_NOFOLLOW_ANY` - "f"
  - `O_EXEC` - "e"
  - `O_SEARCH` - "s"


### TODO:
- fix the handling of long paths (especially in `rename()`)

