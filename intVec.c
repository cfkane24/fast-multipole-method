typedef struct
{
  int i;
  int j;
  int k;
}intVec;


intVec getIJKVec(int n, int reg){

  intVec thot;
  int nSq = n*n;
  thot.k = reg/nSq;//Make nSq instead of doing reg/n/n to avoid bad rounding
  int blah = reg - nSq*thot.k;//Give me what's left of reg after subtracting k
  thot.j = blah/n;
  thot.i = reg - thot.k - thot.j;
  return(thot);
}
