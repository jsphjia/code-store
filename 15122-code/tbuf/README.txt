15-122 Principles of Imperative Computation
Text Buffers

==========================================================

Files you won't modify:
   tbuf-test.c0     - Test adapter for text buffers
   editor-test.c0   - Test adapter for editor
   test-main.c0     - Main test program
   lovas-E0.c0      - The E0 text editor implementation

Files you will modify:
   tbuf.c0          - Text buffer implemented as a doubly-linked lists of chars
   test-is_tbuf.c0  - Test for is_tbuf (optional, Task 1)
   editor.c0        - Text buffers augmented with position tracking

==========================================================

Testing is_tbuf:
   % cc0 -d -W -w -x tbuf.c0 test-is_tbuf.c0

Compiling and running the tbuf tests:
   % cc0 -d -W -w -o tbuf-test tbuf.c0 tbuf-test.c0 test-main.c0
   % ./tbuf-test

Compiling and running the editor tests:
   % cc0 -d -W -w -o editor-test tbuf.c0 editor.c0 editor-test.c0 test-main.c0
   % ./editor-test

Compiling and running the E0 editor:
   % cc0 -d -W -w -o E0 tbuf.c0 editor.c0 lovas-E0.c0
   % ./E0

==========================================================

Submitting from the command line on andrew:
   % autolab122 handin tbuf tbuf.c0 editor.c0
then display autolab's feedback by running:
   % autolab122 feedback

Creating a tarball to submit with autolab.andrew.cmu.edu web interface:
   % tar -czvf handin.tgz tbuf.c0 editor.c0
