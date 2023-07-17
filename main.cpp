#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

typedef int tImage[1000][1000];

string error;

int loadPGM(string name, tImage img, int *lin, int *col, int *tone)
{

    string type;
    ifstream file(name);
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

int savePGM(string name, tImage img, int lin, int col, int tone)
{
    ofstream file(name);
    if (!file.is_open())
    {
        error = "Erro: Arquivo não encontrado.";
        return 1;
    }

    file << "P2" << endl
         << col << " " << lin << endl
         << tone << endl;
    for (int i = 0; i < lin; i++)
    {
        for (int j = 0; j < col; j++)
        {
            file << img[i][j] << " ";
        }
        file << endl;
    }

    file.close();
    return 0;
}

/*
 * Leitura e Escrita de arquivos no formato PGM com funções.
 */
int main(int argc, char **argv)
{
    tImage input_img, output_img;
    int columns = 10, lines = 10, tone;
    string input_file, output_file;

    // Leitura do arquivo de entrada da imagem.
    cout << " Entre com o nome da imagem de entrada: ";
    cin >> input_file;
    input_file = input_file + ".pgm";
    if (loadPGM(input_file, input_img, &lines, &columns, &tone) != 0)
    {
        cout << "\n" + error + "\n";
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
    cout << " Entre com o nome da imagem de saída: ";
    cin >> output_file;
    output_file = output_file + ".pgm";
    if (savePGM(output_file, output_img, columns, lines, tone) != 0)
    {
        cout << "\n" + error + "\n";
        return 1;
    }

    return 0;
}