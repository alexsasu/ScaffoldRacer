#include "Consts.h"
#include "MatrixUtils.h"

void MatrixUtils::displayMatrixIcon(GameAssets &gameAssets, uint64_t matrixIcon) {
  for (int i = 0; i < matrixIconBytesCount; ++i) {
    byte row = (matrixIcon >> i * matrixIconBytesCount) & 0xFF;
    
    for (int j = 0; j < matrixIconBytesCount; ++j) {
      gameAssets.matrixSetLed(0, i, j, bitRead(row, j));
    }
  }
}