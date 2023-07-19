#include <cstdlib>
#include <iostream>
#include <fstream>

typedef int tImage[1000][1000];

std::string error;

int loadPGM(std::string name, tImage iImg, int *lin, int *col, int *tone)
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
            file >> iImg[i][j];
        }
    }
    file.close();
    return 0;
}

int savePGM(std::string name, tImage oImg, int lin, int col, int tone)
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
            file << oImg[i][j] << " ";
        }
        file << std::endl;
    }

    file.close();
    return 0;
}

void rotate(tImage iImg, tImage oImg, int *lin, int *col, int dir)
{
    switch (dir)
    {
    case 0:
        for (int i = 0; i < *lin; i++)
        {
            for (int j = 0; j < *col; j++)
            {
                oImg[i][j] = iImg[j][*col - i];
            }
        }
        break;

    case 1:
        for (int i = 0; i < *lin; i++)
        {
            for (int j = 0; j < *col; j++)
            {
                oImg[i][j] = iImg[*col - j][i];
            }
        }
        break;

    default:
        break;
    }
}

void binarize(tImage iImg, tImage oImg, int *lin, int *col)
{
    for (int i = 0; i < *lin; i++)
    {
        for (int j = 0; j < *col; j++)
        {
            if (iImg[i][j] < 128)
            {
                oImg[i][j] = 0;
            }
            else
            {
                oImg[i][j] = 255;
            }
        }
    }
}

void view(tImage iImg, tImage oImg, int *lin, int *col)
{
    for (int i = 0; i < *lin; i++)
    {
        for (int j = 0; j < *col; j++)
        {
            oImg[i][j] = iImg[i][j];
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
    tImage input_image, output_image;
    int columns, lines, tone, option;
    std::string input_file, output_file;

    // Leitura do arquivo de entrada da imagem.
    std::cout << " Entre com o nome da imagem de entrada: ";
    std::cin >> input_file;
    input_file += ".pgm";

    if (loadPGM(input_file, input_image, &lines, &columns, &tone) != 0)
    {
        std::cout << "\n" + error + "\n";
        return 1;
    }

    std::cout << "O que quer fazer com a imagem?" << std::endl
              << std::endl
              << "0-Rotacionar a imagem" << std::endl
              << "1-Binarizar a imagem" << std::endl
              << "2-Iconizar a imagem" << std::endl
              << "3-Aplicar filtro passa-baida" << std::endl
              << "N/a-Copiar a imagem" << std::endl;
    std::cin >> option;

    switch (option)
    {
    case 0:
        int direction;
        std::cout << "Escolha para qual direção rotacionar" << std::endl
                  << "0-Esquerda" << std::endl
                  << "1-Direita" << std::endl;
        std::cin >> direction;
        rotate(input_image, output_image, &lines, &columns, direction);
        break;

    case 1:
        binarize(input_image, output_image, &lines, &columns);
        break;

    case 2:
        break;

    case 3:
        break;

    default:
        view(input_image, output_image, &lines, &columns);
        break;
    }

    // Escrita do arquivo de saída da imagem.
    std::cout << " Entre com o nome da imagem de saída: ";
    std::cin >> output_file;
    output_file = output_file + ".pgm";
    if (savePGM(output_file, output_image, columns, lines, tone) != 0)
    {
        std::cout << "\n" + error + "\n";
        return 1;
    }

    return 0;
}