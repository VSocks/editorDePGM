#include <cstdlib>
#include <iostream>
#include <fstream>

typedef int tImage[1000][1000];

std::string error;

int loadPGM(std::string name, tImage img, int *lin, int *col, int *tone)
{

    std::string type;
    std::ifstream file(name);
    if (!file.is_open())
    {
        error = "Erro: Arquivo não encontrado.";
        return 1;
    }

    file >> type;
    if (type != "P2")
    {
        error = "Erro: Arquivo não tem formato P2.";
        return 2;
    }

    file >> *col >> *lin >> *tone;
    for (int i = 0; i < *lin; i++)
    {
        for (int j = 0; j < *col; j++)
        {
            file >> img[i][j];
        }
    }
    file.close();
    return 0;
}

int savePGM(std::string name, tImage img, int lin, int col, int tone)
{
    std::ofstream file(name);
    if (!file.is_open())
    {
        error = "Erro: Arquivo não encontrado.";
        return 1;
    }

    file << "P2" << std::endl
         << col << " " << lin << std::endl
         << tone << std::endl;
    for (int i = 0; i < lin; i++)
    {
        for (int j = 0; j < col; j++)
        {
            file << img[i][j] << " ";
        }
        file << std::endl;
    }

    file.close();
    return 0;
}

/*
 * Leitura e Escrita de arquivos no formato PGM com funções.
 */
int main()
{
    tImage input_img, output_img;
    int columns = 10, lines = 10, tone;
    std::string input_file, output_file;

    // Leitura do arquivo de entrada da imagem.
    std::cout << " Entre com o nome da imagem de entrada: ";
    std::cin >> input_file;
    input_file = input_file + ".pgm";
    if (loadPGM(input_file, input_img, &lines, &columns, &tone) != 0)
    {
        std::cout << "\n" + error + "\n";
        return 1;
    }

    // Construindo a imagem negativa.
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            input_img[j][i] = tone - output_img[i][j];
        }
    }

    // Escrita do arquivo de saída da imagem.
    std::cout << " Entre com o nome da imagem de saída: ";
    std::cin >> output_file;
    output_file = output_file + ".pgm";
    if (savePGM(output_file, output_img, columns, lines, tone) != 0)
    {
        std::cout << "\n" + error + "\n";
        return 1;
    }

    return 0;
}