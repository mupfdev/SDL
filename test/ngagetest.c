// The purpose of this test is to validate that there are no undefined references
// when building with the N-Gage SDK since SDL can only be linked statically.

int SDL_main(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++) { // C99 check.
        /* Nothing to do here. */
    }

    return 0;
}
