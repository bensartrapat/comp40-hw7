/* 
 * 	Title: umlabwrite.c 
 * 	Name: Nga Pham, Sartrapat Saengcharoentrakul 
 * 	Username: npham02, ssaeng01
 * 	Assigment 6
 * 
 * 	Note: This code is based of the code given in the lab.
 */


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "fmt.h"
#include "seq.h"

extern void Um_write_sequence(FILE *output, Seq_T asm);

extern void emit_halt_test(Seq_T asm);
extern void emit_LV_Output_test(Seq_T asm);
extern void emit_input_test(Seq_T asm);
extern void emit_add_test(Seq_T asm);
extern void emit_mult_test(Seq_T asm);
extern void emit_div_test(Seq_T asm);
extern void emit_nand_test(Seq_T asm);
extern void emit_conditionalMove_test(Seq_T asm);
extern void emit_goto_test(Seq_T asm);
extern void emit_map_SL_SS_test(Seq_T asm);
extern void emit_half_million(Seq_T asm);

static struct test_info {
        const char *name;
        const char *test_input;            /* NULL means no input needed */
        const char *expected_output;
        /* writes instructions into sequence */
        void (*emit_test) (Seq_T stream);
} tests[] = {
        {"halt", NULL, "", emit_halt_test},
        {"LVOutput", NULL, "Good!", emit_LV_Output_test},
        {"input", "2", "2", emit_input_test},
        {"add", NULL, "2", emit_add_test},
        {"mult", NULL, "@", emit_mult_test},
        {"div", NULL, "2", emit_div_test},
        {"nand", NULL, "", emit_nand_test},
	{"cm", NULL, "3", emit_conditionalMove_test},
	{"goto", NULL, "GOTO passed.\n", emit_goto_test},                
        {"mapSLSS", NULL, "BEE", emit_map_SL_SS_test},        
        {"half_million", NULL, "", emit_half_million},
};

#define NTESTS (sizeof(tests)/sizeof(tests[0]))

/*
 * open file 'path' for writing, then free the pathname;
 * if anything fails, checked runtime error
 */
static FILE *open_and_free_pathname(char *path);

/*
 * if contents is NULL or empty, remove the given 'path', 
 * otherwise write 'contents' into 'path'.  Either way, free 'path'.
 */
static void write_or_remove_file(char *path, const char *contents);

static void write_test_files(struct test_info *test)
{
        FILE *binary = open_and_free_pathname(Fmt_string("%s.um", test->name));
        Seq_T asm = Seq_new(0);
        test->emit_test(asm);
        Um_write_sequence(binary, asm);
        Seq_free(&asm);
        fclose(binary);

        write_or_remove_file(Fmt_string("%s.0", test->name), test->test_input);
        write_or_remove_file(Fmt_string("%s.1", test->name),
                             	 	 	 	test->expected_output);
}

int main(int argc, char *argv[])
{
        bool failed = false;
        if (argc == 1)
                for (unsigned i = 0; i < NTESTS; i++) {
                        printf("***** Writing test '%s'.\n", tests[i].name);
                        write_test_files(&tests[i]);
        } else
                for (int j = 1; j < argc; j++) {
                        bool tested = false;
                        for (unsigned i = 0; i < NTESTS; i++)
                                if (!strcmp(tests[i].name, argv[j])) {
                                        tested = true;
                                        write_test_files(&tests[i]);
                                }
                        if (!tested) {
                                failed = true;
                                fprintf(stderr,
                                        "***** No test named %s *****\n",
                                        argv[j]);
                        }
                }
        return failed;   /* failed nonzero == exit nonzero == failure */
}

static void write_or_remove_file(char *path, const char *contents)
{
        if (contents == NULL || *contents == '\0') {
                remove(path);
        } else {
                FILE *input = fopen(path, "wb");
                assert(input);
                fputs(contents, input);
                fclose(input);
        }
        free(path);
}

static FILE *open_and_free_pathname(char *path)
{
        FILE *fp = fopen(path, "wb");
        assert(fp != NULL);

        free(path);
        return fp;
}
