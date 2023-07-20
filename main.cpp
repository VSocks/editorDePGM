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
 * Função que rotaciona os valores da matriz imagem input
 * e coloca suas novas posições na matriz imagen output
 */
void rotate(tImage iImg, tImage oImg, int *lin, int *col, int dir)
{
    switch (dir)
    {
    case 0: // Roda os valores da matriz 90 graus à esquerda
        for (int i = 0; i < *col; i++)
        {
            for (int j = 0; j < *lin; j++)
            {
                oImg[i][j] = iImg[j][*col - i];
            }
        }
        break;

    case 1: // Roda os valores da matriz 90 graus à direita
        for (int i = 0; i < *col; i++)
        {
            for (int j = 0; j < *lin; j++)
            {
                oImg[i][j] = iImg[*lin - j][i];
            }
        }
        break;

    default:
        break;
    }
    std::swap(*lin, *col);
}

/*
 * Lê cada valor da imagem input e compara com o valor desejado pelo usuário
 * para a binarização. Valores maiores são preenchidos por branco na imagem output
 * enquanto valores menores são preenchidos com preto
 */
void binarize(tImage iImg, tImage oImg, int *lin, int *col, int dsr_tone)
{
    for (int i = 0; i < *lin; i++)
    {
        for (int j = 0; j < *col; j++)
        {
            if (iImg[i][j] < dsr_tone)
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

void iconize()
{
}

void smooth()
{
}

// Coloca valores invertidos da matriz imagem output na matriz imagem output
void negative(tImage iImg, tImage oImg, int *lin, int *col, int *tone)
{
    for (int i = 0; i < *lin; i++)
    {
        for (int j = 0; j < *col; j++)
        {
            oImg[i][j] = *tone - iImg[i][j];
        }
    }
}

// Esclarese ou escurece a imagem de acordo com o que o usuário especifica
void shade(tImage iImg, tImage oImg, int *lin, int *col, int shade)
{
    if (shade < 0)
    {
        for (int i = 0; i < *lin; i++)
        {
            for (int j = 0; j < *col; j++)
            {
                if (iImg[i][j] + shade < 0)
                {
                    oImg[i][j] = 0;
                }
                else
                {
                    oImg[i][j] = iImg[i][j] + shade;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < *lin; i++)
        {
            for (int j = 0; j < *col; j++)
            {
                if (iImg[i][j] + shade > 255)
                {
                    oImg[i][j] = 255;
                }
                else
                {
                    oImg[i][j] = iImg[i][j] + shade;
                }
            }
        }
    }
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

    std::cout << std::endl
              << "O que quer fazer com a imagem?" << std::endl
              << std::endl
              << "0-Rotacionar a imagem" << std::endl
              << "1-Binarizar a imagem" << std::endl
              << "2-Iconizar a imagem (64x64)" << std::endl
              << "3-Aplicar filtro passa-baida" << std::endl
              << "4-Inverter cores da imagem" << std::endl
              << "5-Escurecer ou clarear a imagem" << std::endl
              << "N/a-Copiar a imagem" << std::endl
              << "Opção: ";
    std::cin >> option;

    switch (option)
    {
    case 0:
        int direction;
        std::cout << std::endl
                  << "Escolha para qual direção rotacionar" << std::endl
                  << "0-Esquerda" << std::endl
                  << "1-Direita" << std::endl
                  << "Opção: ";
        std::cin >> direction;
        rotate(input_image, output_image, &lines, &columns, direction);
        break;

    case 1:
        int desired_tone;
        std::cout << std::endl
                  << "Digite o valor do tom de cinza desejado para binarização, entre 0 e 255" << std::endl
                  << "Tons de mais baixo valor serão convertidos em preto" << std::endl
                  << "Tons de igual ou mais alto valor serão convertidos em branco" << std::endl
                  << "Tom desejado: ";
        std::cin >> desired_tone;
        binarize(input_image, output_image, &lines, &columns, desired_tone);
        break;

    case 2:
        iconize();
        break;

    case 3:
        smooth();
        break;

    case 4:
        negative(input_image, output_image, &lines, &columns, &tone);
        break;

    case 5:
        int shading;
        std::cout << std::endl
                  << "Digite o valor desejado de tons a escurecer ou clarear a imagem" << std::endl
                  << "Valores negativos para escurecer, valores positivos para clarear" << std::endl
                  << "Os tons usados na imagem vão de 0 (preto) a 255 (branco)" << std::endl
                  << "Mudança desejada: ";
        std::cin >> shading;
        shade(input_image, output_image, &lines, &columns, shading);
        break;

    default:
        copy(input_image, output_image, &lines, &columns);
        break;
    }

    // Escrita do arquivo de saída da imagem.
    std::cout << std::endl
              << "Entre com o nome da imagem de saída: ";
    std::cin >> output_file;
    output_file += ".pgm";
    if (savePGM(output_file, output_image, lines, columns, tone) != 0)
    {
        std::cout << "\n" + error + "\n";
        return 1;
    }

    return 0;
}