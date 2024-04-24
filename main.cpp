#include <cstdlib>
#include <iostream>
#include <fstream>

typedef int tImage[1000][1000];

std::string error;

// Cria matriz "imagem input" com os valores do arquivo de entrada
// esse arquivo será então processado para formar a matriz "imagem output"
int loadPGM(std::string name, tImage iImg, int *lin, int *col, int *tone){
    std::string type;
    std::ifstream file(name);

    if(!file.is_open()){
        error = "Erro: Arquivo não encontrado.";
        return 1;
    }

    file >> type;
    if(type != "P2"){
        error = "Erro: Arquivo não tem formato P2.";
        return 1;
    }

    file >> *col >> *lin >> *tone;
    for(int i = 0; i < *lin; i++)
        for(int j = 0; j < *col; j++)
            file >> iImg[i][j];

    file.close();
    return 0;
}

// Cria arquivo pgm baseado na matriz "imagem output"
int savePGM(std::string name, tImage oImg, int lin, int col, int tone){
    std::ofstream file(name);
    if(!file.is_open()){
        error = "Erro: Arquivo não encontrado.";
        return 1;
    }

    file << "P2\n"
         << col << " " << lin << '\n'
         << tone << '\n';
    for(int i = 0; i < lin; i++){
        for(int j = 0; j < col; j++)
            file << oImg[i][j] << " ";
        file << '\n';
    }

    file.close();
    return 0;
}

// Função que rotaciona os valores da matriz imagem input
// e coloca suas novas posições na matriz imagen output
int rotate(tImage iImg, tImage oImg, int *lin, int *col, int dir, std::string *msg){
    switch(dir){
        case 0: // Roda os valores da matriz 90 graus à esquerda
            *msg = "esquerda";
            for (int i = 0; i < *col; i++)
                for (int j = 0; j < *lin; j++)
                    oImg[i][j] = iImg[j][*col - i];
            break;

        case 1: // Roda os valores da matriz 90 graus à direita
            *msg = "direita";
            for (int i = 0; i < *col; i++)
                for (int j = 0; j < *lin; j++)
                    oImg[i][j] = iImg[*lin - j][i];
            break;

        default:
            error = "Erro: opção inválida.";
            return 1;
    }
    std::swap(*lin, *col);
    return 0;
}

// Lê cada valor da imagem input e compara com o valor desejado pelo usuário
// para a binarização. Valores maiores são preenchidos por branco na imagem output
// enquanto valores menores são preenchidos com preto
int binarize(tImage iImg, tImage oImg, int *lin, int *col, int dsr_tone){
    if(dsr_tone > 255 || dsr_tone < 0){
        error = "Erro: valor inválido.";
        return 1;
    }
    else
        for(int i = 0; i < *lin; i++)
            for(int j = 0; j < *col; j++)
                if(iImg[i][j] < dsr_tone)
                    oImg[i][j] = 0;
                else
                    oImg[i][j] = 255;

    return 0;
}

/*
 * Cria um ícone 64x64 da imagem, calculando a cada quantos pixeis de altura
 * ou comprimento da imagem real um novo píxel da imagem iconizada deve ser escolhido.
 * Isso acaba "esticando" as proporções de imagens que não são quadradas
 */
void iconize(tImage iImg, tImage oImg, int *lin, int *col){
    for(int i = 0; i < 64; i++)
        for(int j = 0; j < 64; j++)
            oImg[i][j] = iImg[i * (*lin / 64)][j * (*col / 64)];

    *lin = 64;
    *col = 64;
}

// Função que aplica filtro passa-baixa
void smooth(tImage iImg, tImage oImg, int *lin, int *col){
    int matrix[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

    for(int i = 0; i < *lin; i++)
        for(int j = 0; j < *col; j++){
            int sum1 = 0;
            for(int k = -1; k <= 1; k++){
                for(int l = -1; l <= 1; l++)
                    sum1 += iImg[i + 1][j + l] * matrix[k + 1][l + 1];
            }
            oImg[i][j] = sum1 / 9;
        }
}

// Coloca valores invertidos da matriz imagem output na matriz imagem output
void negative(tImage iImg, tImage oImg, int *lin, int *col, int *tone){
    for (int i = 0; i < *lin; i++)
        for (int j = 0; j < *col; j++)
            oImg[i][j] = *tone - iImg[i][j];
}

// Esclarese ou escurece a imagem de acordo com o que o usuário especifica
void shade(tImage iImg, tImage oImg, int *lin, int *col, int shade, std::string *msg){
    if(shade < 0){
        *msg = "escurecida";
        for(int i = 0; i < *lin; i++)
            for(int j = 0; j < *col; j++)
                if (iImg[i][j] + shade < 0)
                    oImg[i][j] = 0;
                else
                    oImg[i][j] = iImg[i][j] + shade;
    }else{
        *msg = "clareada";
        for(int i = 0; i < *lin; i++)
            for(int j = 0; j < *col; j++)
                if(iImg[i][j] + shade > 255)
                    oImg[i][j] = 255;
                else
                    oImg[i][j] = iImg[i][j] + shade;
    }
}

// Inverte a orientação da imagem verticalmente ou horizontalmente
void flip(tImage iImg, tImage oImg, int *lin, int *col, int orien, std::string *msg){
    switch(orien){
        //inverter horizontal
        case 0:
            *msg = "horizontalmente";
            for(int i = 0; i < *lin; i++)
                for(int j = 0; j < *col; j++)
                    oImg[i][j] = iImg[i][*col - j];
            break;
        //inverter vertical
        case 1:
            *msg = "verticalmente";
            for(int i = 0; i < *lin; i++)
                for(int j = 0; j < *col; j++)
                    oImg[i][j] = iImg[*lin - i][j];
            break;
    }
}

// Salva alterações na imagem de input toda vez que uma alteração for feita na imagem output.
// Assim é possível fazer várias alterações em sequência, uma vez que nas outras funções as
// mudanças da imagem de output usam a imagem input como base
void saveChanges(tImage iImg, tImage oImg, int *lin, int *col){
    for(int i = 0; i < *lin; i++)
        for (int j = 0; j < *col; j++)
            iImg[i][j] = oImg[i][j];
}

/*
 * Leitura e Escrita de arquivos no formato PGM com funções.
 */

int main(){
    tImage input_image, output_image;
    int columns, lines, tone, option;
    std::string input_file, output_file, message;

    // Leitura do arquivo de entrada da imagem.
    std::cout << " Entre com o nome da imagem de entrada: ";
    std::cin >> input_file;
    input_file += ".pgm";

    if(loadPGM(input_file, input_image, &lines, &columns, &tone) != 0){
        std::cout << "\n" + error + "\n";
        return 1;
    }

    while(option != 7){
        std::cout << "\nO que quer fazer com a imagem?\n\n"
                  << "0-Rotacionar a imagem\n"
                  << "1-Binarizar a imagem\n"
                  << "2-Iconizar a imagem (64x64)\n"
                  << "3-Aplicar filtro passa-baixa\n"
                  << "4-Inverter cores da imagem\n"
                  << "5-Escurecer ou clarear a imagem\n"
                  << "6-Inverter orientação da imagem\n"
                  << "7-Finalizar\n"
                  << "Opção: ";
        std::cin >> option;

        switch (option){
            case 0:
                int direction;
                std::cout << "\nEscolha para qual direção rotacionar\n"
                        << "0-Esquerda\n"
                        << "1-Direita\n"
                        << "Opção: ";
                std::cin >> direction;
                if(rotate(input_image, output_image, &lines, &columns, direction, &message) != 0){
                    std::cout << "\n" + error + "\n";
                    return 1;
                }
                saveChanges(input_image, output_image, &lines, &columns);
                std::cout << "\nImagem rotacionada à " << message << " com sucesso";
                break;

            case 1:
                int desired_tone;
                std::cout << "\nDigite o valor do tom de cinza desejado para binarização, entre 0 e 255\n"
                        << "Tons de mais baixo valor serão convertidos em preto\n"
                        << "Tons de igual ou mais alto valor serão convertidos em branco\n"
                        << "Tom desejado: ";
                std::cin >> desired_tone;
                if(binarize(input_image, output_image, &lines, &columns, desired_tone) != 0){
                    std::cout << "\n" + error + "\n";
                    return 1;
                }
                saveChanges(input_image, output_image, &lines, &columns);
                std::cout << "\nImagem binarizada com sucesso.";
                break;

            case 2:
                iconize(input_image, output_image, &lines, &columns);
                saveChanges(input_image, output_image, &lines, &columns);
                std::cout << "\nImagem iconizada com sucesso.";
                break;

            case 3:
                smooth(input_image, output_image, &lines, &columns);
                saveChanges(input_image, output_image, &lines, &columns);
                std::cout << "\nImagem suavizada com sucesso.";
                break;

            case 4:
                negative(input_image, output_image, &lines, &columns, &tone);
                saveChanges(input_image, output_image, &lines, &columns);
                std::cout << "\nImagem negativada com sucesso.";
                break;

            case 5:
                int shading;
                std::cout << "\nDigite o valor desejado de tons a escurecer ou clarear a imagem\n"
                        << "Valores negativos para escurecer, valores positivos para clarear\n"
                        << "Os tons usados na imagem vão de 0 (preto) a 255 (branco)\n"
                        << "Mudança desejada: ";
                std::cin >> shading;
                shade(input_image, output_image, &lines, &columns, shading, &message);
                saveChanges(input_image, output_image, &lines, &columns);
                std::cout << "\nImagem " << message << " com sucesso.";
                break;

            case 6:
                int orientation;
                std::cout << "\nEscolha qual orientação inverter\n"
                        << "0-Inverter horizontalmente\n"
                        << "1-Inverter verticalmente\n"
                        << "Opção: ";
                std::cin >> orientation;
                flip(input_image, output_image, &lines, &columns, orientation, &message);
                saveChanges(input_image, output_image, &lines, &columns);
                std::cout << "\nImagem invertida " << message << " com sucesso.";
                break;

            case 7:
                // Escrita do arquivo de saída da imagem.
                std::cout << "\nEntre com o nome da imagem de saída: ";
                std::cin >> output_file;
                output_file += ".pgm";
                if(savePGM(output_file, output_image, lines, columns, tone) != 0){
                    std::cout << "\n" + error + "\n";
                    return 1;
                }
                return 0;
                break;

            default:
                std::cout << "\nErro: opção inválida.\n";
                return 1;
        }
    }
    return 0;
}