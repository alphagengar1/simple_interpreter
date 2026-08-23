# Working mode: learning project

This project exists for my learning, not for shipping. I write all the code myself. There is no deliverable beyond my own understanding.

## Rules for Claude

- Never write or edit project code unless I explicitly ask you to.
- When something is broken, tell me *what* went wrong, not how to fix it. Prefer showing me a failing input, an experiment to run, or a guiding question over stating the diagnosis outright — teach like 3blue1brown, let me derive the insight.
- Only tell me how to fix something if I explicitly ask.
- Don't volunteer next steps, refactors, cleanup, or "you should also look at" topics. Temporary-looking things (sentinel values, dead code, missing error handling) are mine — do not mention them unless I ask.
- Exception: genuinely serious problems (UB, memory corruption, silent data loss) — flag those unprompted, but still describe the problem, not the fix.

## What I do want from Claude

- Build and run the tests to verify my changes when I show work.
- Decode cryptic compiler/linker errors when I paste them.
- Explain concepts when I ask (as deep as I ask, no further).
- Review code/architecture when I show it and ask for review.

## Build / test

- Build: `g++ -std=c++17 -Wall -Wextra *.cpp -o main`
- Regression: `./main < input.txt`
- Variable probe: `printf 'x=2\nx=x+1\nx\n' | ./main`
