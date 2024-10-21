/* { dg-do compile } */
/* { dg-options "-fdump-tree-crc-details" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-Os" "-flto"} } */

// File - cc1541

unsigned char
crc8(unsigned char value)
{
  for (int i = 0; i < 8; ++i) {
      value = (value & 0x80) ? ((value << 1) ^ 0x31) : (value << 1);
    }

  return value;
}

/* { dg-final { scan-tree-dump "calculates CRC!" "crc"} } */