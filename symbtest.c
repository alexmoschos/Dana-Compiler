/******************************************************************************
 *  CVS version:
 *     $Id: symbtest.c,v 1.1 2003/05/13 22:21:01 nickie Exp $
 ******************************************************************************
 *
 *  C code file : symbtest.c
 *  Project     : PCL Compiler
 *  Version     : 1.0 alpha
 *  Written by  : Nikolaos S. Papaspyrou (nickie@softlab.ntua.gr)
 *  Date        : May 14, 2003
 *  Description : Generic symbol table in C, demo program
 *
 *  Comments: (in Greek iso-8859-7)
 *  ---------
 *  Εθνικό Μετσόβιο Πολυτεχνείο.
 *  Σχολή Ηλεκτρολόγων Μηχανικών και Μηχανικών Υπολογιστών.
 *  Τομέας Τεχνολογίας Πληροφορικής και Υπολογιστών.
 *  Εργαστήριο Τεχνολογίας Λογισμικού
 */


/* Header files */

#include <stdlib.h>
#include <stdio.h>

#include "symbol.h"


/* Διαδικασία εκτύπωσης του πίνακα συμβόλων */

#define SHOW_OFFSETS

void printSymbolTable ()
{
    Scope       * scp;
    SymbolEntry * e;
    SymbolEntry * args;
    
    scp = currentScope;
    if (scp == NULL)
        printf("no scope\n");
    else
        while (scp != NULL) {
            printf("scope: ");
            e = scp->entries;
            while (e != NULL) {
                if (e->entryType == ENTRY_TEMPORARY)
                    printf("$%d", e->u.eTemporary.number);
                else
                    printf("%s", e->id);
                switch (e->entryType) {
                    case ENTRY_FUNCTION:
                        printf("(");
                        args = e->u.eFunction.firstArgument;
                        while (args != NULL) {
                            printMode(args->u.eParameter.mode);
                            printf("%s : ", args->id);
                            printType(args->u.eParameter.type);
                            args = args->u.eParameter.next;
                            if (args != NULL)
                                printf("; ");
                        }
                        printf(") : ");
                        printType(e->u.eFunction.resultType);
                        break;
#ifdef SHOW_OFFSETS
                    case ENTRY_VARIABLE:
                        printf("[%d]", e->u.eVariable.offset);
                        break;
                    case ENTRY_PARAMETER:
                        printf("[%d]", e->u.eParameter.offset);
                        break;
                    case ENTRY_TEMPORARY:
                        printf("[%d]", e->u.eTemporary.offset);
                        break;
#endif
                }
                e = e->nextInScope;
                if (e != NULL)
                    printf(", ");
            }
            scp = scp->parent;
            printf("\n");
        }
    printf("----------------------------------------\n");
}


/* Κύριο πρόγραμμα επίδειξης του πίνακα συμβόλων */

/* Ακολουθεί ο κώδικας ενός προγράμματος PCL που χρησιμοποιείται
   για τον έλεγχο του πίνακα συμβόλων.

   program p;

      var s1, s2, s3 : real;
          i1, i2 : integer;

      forward procedure f (x : integer; var y : real);

      procedure pr (p1, p2 : integer; var p3 : boolean)
         var b1 : boolean;
             i1 : integer;
      begin (* of procedure pr        *)
            (* access s1, i2, i1      *)
            (* make 2 new temporaries *)
      end;  (* of procedure pr        *)

      procedure f (x : integer; var y : real);
      begin (* of procedure f         *)
            (* ... whatever ...       *)
      end;  (* of procedure f         *)

   begin    (* of program p           *)
            (* make 2 new temporaries *)
   end.     (* of program p           *)
*/

int main ()
{
   /* Τοπικές μεταβλητές */

   SymbolEntry * p;

   /* Αρχικοποίηση ! */

   initSymbolTable(256);

   printSymbolTable();

/* SOURCE : program p; */

   openScope();

   printSymbolTable();

/* SOURCE : var s1, s2, s3 : real; */
      
   newVariable("s1", typeReal);
   newVariable("s2", typeReal);
   newVariable("s3", typeReal);

   printSymbolTable();

/* SOURCE : i1, i2 : integer; */

   newVariable("i1", typeInteger);
   newVariable("i2", typeInteger);

   printSymbolTable();

/* SOURCE : forward procedure f ( ... */

   p = newFunction("f");
   forwardFunction(p);
   openScope();
   
   printSymbolTable();

/* SOURCE : x : integer; var y : real); */

   newParameter("x", typeInteger, PASS_BY_VALUE, p);
   newParameter("y", typeReal, PASS_BY_REFERENCE, p);
   endFunctionHeader(p, typeVoid);

   printSymbolTable();

   closeScope();

   printSymbolTable();

/* SOURCE : procedure pr (... */

   p = newFunction("pr");
   openScope();

   printSymbolTable();

/* SOURCE : p1, p2 : integer; var p3 : boolean) */
      
   newParameter("p1", typeInteger, PASS_BY_VALUE, p);
   newParameter("p2", typeInteger, PASS_BY_VALUE, p);
   newParameter("p3", typeBoolean, PASS_BY_REFERENCE, p);
   endFunctionHeader(p, typeVoid);

   printSymbolTable();

/* SOURCE : var b1 : boolean; */

   newVariable("b1", typeBoolean);

/* SOURCE : i1 : integer; */

   newVariable("i1", typeInteger);

   printSymbolTable();

/* SOURCE : (* access s1, i2, i1 *) */

   lookupEntry("s1", LOOKUP_ALL_SCOPES, true);
   lookupEntry("i2", LOOKUP_ALL_SCOPES, true);
   lookupEntry("i1", LOOKUP_ALL_SCOPES, true);

/* SOURCE : (* make 2 new temporaries *) */

   newTemporary(typeInteger);
   newTemporary(typeBoolean);

   printSymbolTable();

/* SOURCE : end; (* of procedure pr *) */

   closeScope();

   printSymbolTable();

/* SOURCE : procedure f ( ... */
      
   p = newFunction("f");
   openScope();

   printSymbolTable();

/* SOURCE : x : integer; var y : real); */

   newParameter("x", typeInteger, PASS_BY_VALUE, p);
   newParameter("y", typeReal, PASS_BY_REFERENCE, p);
   endFunctionHeader(p, typeVoid);

   printSymbolTable();

/* SOURCE : end; (* of procedure f *) */

   closeScope();

   printSymbolTable();

/* SOURCE : (* make 2 new temporaries *) */

   newTemporary(typeInteger);
   newTemporary(typeInteger);

   printSymbolTable();

/* SOURCE : end. (* of program p *) */

   closeScope();

   printSymbolTable();

   return 0;
}
