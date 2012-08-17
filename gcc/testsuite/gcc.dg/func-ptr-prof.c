/* { dg-require-profiling "-pg" } */
/* { dg-options "-pg" } */

extern void exit (int);

void
f (void)
{
}

void (*fp) (void) = &f;

int
main (int argc, char **argv)
{
  fp ();
  exit (0);
}
