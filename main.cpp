#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <algorithm>

using namespace std;

int main()
{
     // Taille de l'entète format BMP
    size_t headerSize = 54;

    string inputPath = "../BMPin/penguin.bmp";
    ifstream file(inputPath, std::ios::binary);
    if (!file)
    {
        std::cerr << "Erreur : impossible d'ouvrir " << inputPath << std::endl;
        return 1;
    }

    vector<unsigned char> data((std::istreambuf_iterator<char>(file)),
                                    std::istreambuf_iterator<char>());
    file.close();

    cout << "Fichier lu : " << data.size() << " octets.\n";

    size_t n = std::min<size_t>(64, data.size() - headerSize); // limiter à 64 octets

    cout << "\n--- Bits faibles avant modification ---\n";
    for (size_t i = 0; i < n; ++i)
    {
        bitset<8> bits(data[headerSize + i]);
        cout << bits.to_string().substr(4,4) << " ";
        if ((i+1) % 8 == 0) cout << "\n"; // 8 octets par ligne
    }

    // Modifier le LSB (bit faible) de chaque octet
    vector<unsigned char> modifiedData = data;
    for (size_t i = headerSize; i < modifiedData.size(); ++i)
        modifiedData[i] |= 0x01;

    cout << "\n--- Bits faibles apres modification ---\n";
    for (size_t i = 0; i < n; ++i)
    {
        bitset<8> bits(modifiedData[headerSize + i]);
        cout << bits.to_string().substr(4,4) << " ";
        if ((i+1) % 8 == 0) cout << "\n"; // 8 octets par ligne
    }

    // Sauvegarder l'image modifiée
    ofstream outFile("../BMPout/penguin_LSB.bmp", ios::binary);
    outFile.write(reinterpret_cast<char*>(modifiedData.data()), modifiedData.size());
    outFile.close();

    return 0;
}
