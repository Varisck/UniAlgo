#include <chrono>
#include <fstream>
#include <iostream>

#include "unialgo/pattern/wordVecMatching.hpp"
#include "unialgo/utils/bitvector/bitVectors.hpp"
#include "unialgo/utils/waveletMatrix.hpp"

void readFromFile(std::string& s) {
  std::string filename =
      "T:\\Progetti\\UniAlgo\\unialgo\\utils\\test.txt";  // Specify the file
                                                          // name
  std::ifstream file(filename);                           // Open the file

  if (file.is_open()) {
    // Read a single string from the file
    file >> s;

    // Close the file
    file.close();
  } else {
    exit(1);
    std::cerr << "Unable to open file" << std::endl;
  }
}

void makeWv(unialgo::utils::WordVector& wv, std::string& s,
            unialgo::pattern::Alphabet& alph) {
  alph = unialgo::pattern::GetAlphabet(s);
  wv = unialgo::pattern::StringToBitVector(s, alph);
}

int main() {
  std::string s;
  unialgo::utils::WordVector wv;
  unialgo::pattern::Alphabet alph;

  auto start = std::chrono::high_resolution_clock::now();
  readFromFile(s);
  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Time taken to read file: " << duration.count()
            << " milliseconds" << std::endl;
  std::cout << "String size: " << s.size() << std::endl;

  start = std::chrono::high_resolution_clock::now();
  makeWv(wv, s, alph);
  end = std::chrono::high_resolution_clock::now();

  duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Time taken buld wordvector from string: " << duration.count()
            << " milliseconds" << std::endl;
  std::cout << "Wordvector size: " << wv.size() << std::endl;

  start = std::chrono::high_resolution_clock::now();
  unialgo::utils::WaveletMatrix matrix(wv);
  end = std::chrono::high_resolution_clock::now();

  duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Time taken to build wavelet matrix form wordvector: "
            << duration.count() << " milliseconds" << std::endl;

  start = std::chrono::high_resolution_clock::now();
  for (std::size_t i = 0; i < wv.size(); ++i) matrix.rank(alph['A'], i);
  end = std::chrono::high_resolution_clock::now();

  duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Time taken for rank(A, i) with i in [0, size]: "
            << duration.count() << " milliseconds" << std::endl;

  int offCoutn = 0;

  start = std::chrono::high_resolution_clock::now();
  for (int j = 0; j < wv.size(); ++j) {
    std::size_t count = 0;
    for (std::size_t i = 0; i <= j; ++i)
      if (s[i] == 'A') ++count;
    if (count != matrix.rank(alph['A'], j)) ++offCoutn;
  }
  end = std::chrono::high_resolution_clock::now();

  duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Time taken for same rank but naive: " << duration.count()
            << " milliseconds" << std::endl;

  std::cout << "offcoutn: " << offCoutn << std::endl;

  return 0;
}