int sboxArr(int in){
   static int sbox[16] = {
      7, 15, 0, 6, 8, 10, 12, 9, 2, 14, 11, 1, 4, 3, 13, 5
   };
   return sbox[in];
}
