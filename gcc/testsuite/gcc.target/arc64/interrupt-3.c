void __attribute__ ((interrupt("you load too")))
handler1 (void)
{ /* { dg-warning "is not \"ilink" } */
}

void __attribute__ ((interrupt(42)))
hander2 (void)
{ /* { dg-warning "is not a string constant" } */
}
