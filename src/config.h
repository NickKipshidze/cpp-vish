// Black:   \u001b[30m
// Red:     \u001b[31m
// Green:   \u001b[32m
// Yellow:  \u001b[33m
// Blue:    \u001b[34m
// Magenta: \u001b[35m
// Cyan:    \u001b[36m
// White:   \u001b[37m
// Reset:   \u001b[0m

// %u - username
// %d - directory

static const char *propmtFormat =
    "\u001b[31m"
    "┏━━❰\u001b[35m %u \u001b[31m❱━━❬❬\u001b[36m %d \u001b[31m❭❭\n"
    "┗━$ "
    "\u001b[32m";