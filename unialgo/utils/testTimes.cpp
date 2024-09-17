#include <chrono>
#include <fstream>
#include <iostream>

#include "unialgo/pattern/bwt.hpp"
#include "unialgo/pattern/suffixArray.hpp"
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
  bool naiveCount = false;  // timer for the rank's naive count
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

  if (naiveCount) {
    int offCoutn = 0;

    start = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < wv.size(); ++j) {
      std::size_t count = 0;
      for (std::size_t i = 0; i <= j; ++i)
        if (s[i] == 'A') ++count;
      if (count != matrix.rank(alph['A'], j)) ++offCoutn;
    }
    end = std::chrono::high_resolution_clock::now();

    duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken for same rank but naive: " << duration.count()
              << " milliseconds" << std::endl;

    std::cout << "offcoutn: " << offCoutn << std::endl;
  }

  unialgo::utils::WordVector wv2(wv.size() + 1, wv.getWordSize() + 1);
  for (std::size_t i = 0; i < wv.size(); ++i) wv2[i] = wv[i].getValue() + 1;
  wv2[wv.size()] = 0;

  std::cout << "Starting creation Suffix Array: " << std::endl;

  start = std::chrono::high_resolution_clock::now();
  unialgo::utils::WordVector suffixArray =
      unialgo::pattern::makeSuffixArray(wv2);
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Time taken for suffix array: " << duration.count()
            << " milliseconds" << std::endl;

  std::cout << "Starting creation BWT: " << std::endl;

  start = std::chrono::high_resolution_clock::now();
  unialgo::pattern::Bwt bwt(wv2, suffixArray);
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Time taken for BWT: " << duration.count() << " milliseconds"
            << std::endl;

  std::cout << "Starting counts BWT: " << std::endl;

  int searchTimes = 1000;
  std::string pattern = "acgacg";
  auto p = unialgo::pattern::StringToBitVector(pattern);

  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < searchTimes; ++i) bwt.searchPattern(p);
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Time taken for " << searchTimes
            << " counts: " << duration.count() << " milliseconds" << std::endl;

  return 0;
}