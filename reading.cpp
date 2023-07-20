#include <cstdlib>
#include <iostream>
#include <fstream>

typedef int tImage[1000][1000];

std::string error;

/*
 * Cria matriz "imagem input" com os valores do arquivo de entrada
 * esse arquivo será então processado para formar a matriz "imagem output"
 */
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

// Cria arquivo pgm baseado na matriz "imagem output"
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

/*
 * Cria a matriz imagem output baseado na imagem input sem modificações
 * Usado para checar se o programa está lendo arquivos corretamente
 */
void copy(tImage iImg, tImage oImg, int *lin, int *col)
{
    for (int i = 0; i < *lin; i++)
    {
        for (int j = 0; j < *col; j++)
        {
            oImg[i][j] = iImg[i][j];
        }
    }
}

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

    copy(input_image, output_image, &lines, &columns);

    // Escrita do arquivo de saída da imagem.
    std::cout << std::endl
              << " Entre com o nome da imagem de saída: ";
    std::cin >> output_file;
    output_file = output_file + ".pgm";
    if (savePGM(output_file, output_image, columns, lines, tone) != 0)
    {
        std::cout << "\n" + error + "\n";
        return 1;
    }

    return 0;
}