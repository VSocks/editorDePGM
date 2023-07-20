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

void rotate(tImage img, int *lin, int *col)
{
    int n;
    for (int i = 0; i < *lin; i++)
    {
        for (int j = 0 + n; j < *col; j++)
        {
            std::swap(img[i][j], img[j][i]);
        }
    }
}

void binarize(tImage img, int *lin, int *col)
{
    for (int i = 0; i < *col; i++)
    {
        for (int j = 0; j < *lin; j++)
        {
            if(img[i][j] >= 128)
            {
                img[i][j] = 255;
            }
            else
            {
                img[i][j] = 0;
            }
        }
    }
}

/*    // Construindo a imagem negativa.
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            input_img[j][i] = tone - output_img[i][j];
        }
    }*/

/*
 * Leitura e Escrita de arquivos no formato PGM com funções.
 */
int main()
{
    tImage image;
    int columns, lines, tone, option;
    std::string input_file, output_file;

    // Leitura do arquivo de entrada da imagem.
    std::cout << " Entre com o nome da imagem de entrada: ";
    std::cin >> input_file;
    input_file += ".pgm";

    if (loadPGM(input_file, image, &lines, &columns, &tone) != 0)
    {
        std::cout << "\n" + error + "\n";
        return 1;
    }

    std::cout << "O que quer fazer com a imagem?" << std::endl
              << std::endl
              << "1-Rotacionar a imagem" << std::endl
              << "2-Binarizar a imagem" << std::endl
              << "3-Iconizar a imagem" << std::endl
              << "4-Aplicar filtro passa-baida" << std::endl;
    std::cin >> option;

    switch(option)
    {
        case 1: rotate(image, &lines, &columns);
        break;
        case 2: binarize(image, &lines, &columns);
        break;
        case 3:
        break;
        case 4:
        break;
        default:
        break;
    }

    // Escrita do arquivo de saída da imagem.
    std::cout << " Entre com o nome da imagem de saída: ";
    std::cin >> output_file;
    output_file = output_file + ".pgm";
    if (savePGM(output_file, image, columns, lines, tone) != 0)
    {
        std::cout << "\n" + error + "\n";
        return 1;
    }

    return 0;
}