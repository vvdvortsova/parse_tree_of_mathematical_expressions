#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "math_tree.h"

int main(int argc, char** argv) {
    if(argc == 1) {
        printf("\t\t./treeExpr [flag] [outFile] [expr]\n");
        printf("\t-a \tCreates graviz dump and call dot\n \t\tand creates tex dump and call pdflatex\n");
        printf("\t-g \tCreates graviz dump and call dot\n");
        printf("\t-t \tCreates tex dump and call pdflatex\n");
        printf("\t-t \tCreates tex dump and graviz\n \t\tdo not call dot and pdflatex\n");
        printf("\tWithout params is like -a\n");
        printf("\tExamples:\n");
        printf("\t./treeExpr -a test \"1 + 2\"\n");
        printf("\t./treeExpr -a test \"( 1 + 2 )\"\n");
        exit(EXIT_SUCCESS);
    }
    if(argc == 4) {
        char grName[100] = "";
        auto a = getTokens(argv[3]);
        auto tree = buildTree(&a);
        if(strncmp(argv[1], "-a", 2) == 0) {
            sprintf(grName,"%s_grav.dot", argv[2]);
            gravizDump(grName, tree);
            sprintf(grName,"dot -Tpdf %s_grav.dot -o %s_grav.pdf", argv[2], argv[2]);
            system(grName);
            sprintf(grName,"%s_tex.tex", argv[2]);
            latexDump(grName, tree);
            sprintf(grName,"pdflatex -pdf %s_tex.tex", argv[2]);
            system(grName);
        } else if (strncmp(argv[1], "-g", 2) == 0) {
            sprintf(grName,"%s_grav.dot", argv[2]);
            gravizDump(grName, tree);
            sprintf(grName,"dot -Tpdf %s_grav.dot -o %s_grav.pdf", argv[2], argv[2]);
            system(grName);
        } else if (strncmp(argv[1], "-t", 2) == 0) {
            sprintf(grName,"%s_tex.tex", argv[2]);
            latexDump(grName, tree);
            sprintf(grName,"pdflatex -pdf %s_tex.tex", argv[2]);
            system(grName);
        }  else if (strncmp(argv[1], "-v", 2) == 0) {
            sprintf(grName,"%s_grav.dot", argv[2]);
            gravizDump(grName, tree);
            sprintf(grName,"%s_tex.tex", argv[2]);
            latexDump(grName, tree);
        } else {
            printf("UnKnown flag!\n");
            exit(EXIT_FAILURE);
        }
        return EXIT_SUCCESS;
    }
    if(argc == 3) {
        auto a = getTokens(argv[2]);
        auto tree = buildTree(&a);
        char grName[100] = "";
        sprintf(grName,"%s_grav.dot", argv[1]);
        gravizDump(grName, tree);
        sprintf(grName,"dot -Tpdf %s_grav.dot -o %s_grav.pdf", argv[1], argv[1]);
        system(grName);
        sprintf(grName,"%s_tex.tex", argv[1]);
        latexDump(grName, tree);
        sprintf(grName,"pdflatex -pdf %s_tex.tex", argv[1]);
        system(grName);
        return EXIT_SUCCESS;
    }
    printf("Please!Check your arguments!\n");
    exit(EXIT_FAILURE);
}
