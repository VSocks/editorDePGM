#include <cstdlib>
#include <iostream>
#include <fstream>
#include "imageOperations.h"

/*
 * Leitura e Escrita de arquivos no formato PGM com funções.
 */

int main(){
    tImage input_image, output_image;
    int columns, lines, tone, option;
    bool edited = false;
    std::string input_file, output_file, message;

    // Leitura do arquivo de entrada da imagem.
    std::cout << " Entre com o nome da imagem de entrada: ";
    std::cin >> input_file;
    input_file += ".pgm";

    if(loadPGM(input_file, input_image, &lines, &columns, &tone) != 0){
        std::cout << error;
        return 1;
    }

    do{
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
                edited = true;
                int direction;
                std::cout << "\nEscolha para qual direção rotacionar\n"
                        << "0-Esquerda\n"
                        << "1-Direita\n"
                        << "Opção: ";
                std::cin >> direction;
                if(rotate(input_image, output_image, &lines, &columns, direction, &message) != 0){
                    std::cout << error;
                    return 1;
                }
                saveChanges(input_image, output_image, lines, columns);
                std::cout << "\nImagem rotacionada à " << message << " com sucesso";
                break;

            case 1:
                edited = true;
                int desired_tone;
                std::cout << "\nDigite o valor do tom de cinza desejado para binarização, entre 0 e 255\n"
                        << "Tons de mais baixo valor serão convertidos em preto\n"
                        << "Tons de igual ou mais alto valor serão convertidos em branco\n"
                        << "Tom desejado: ";
                std::cin >> desired_tone;
                if(binarize(input_image, output_image, lines, columns, desired_tone) != 0){
                    std::cout << error;
                    return 1;
                }
                saveChanges(input_image, output_image, lines, columns);
                std::cout << "\nImagem binarizada com sucesso.";
                break;

            case 2:
                edited = true;
                iconize(input_image, output_image, &lines, &columns);
                saveChanges(input_image, output_image, lines, columns);
                std::cout << "\nImagem iconizada com sucesso.";
                break;

            case 3:
                edited = true;
                smooth(input_image, output_image, lines, columns);
                saveChanges(input_image, output_image, lines, columns);
                std::cout << "\nImagem suavizada com sucesso.";
                break;

            case 4:
                edited = true;
                negative(input_image, output_image, lines, columns, &tone);
                saveChanges(input_image, output_image, lines, columns);
                std::cout << "\nImagem negativada com sucesso.";
                break;

            case 5:
                edited = true;
                int shading;
                std::cout << "\nDigite o valor desejado de tons a escurecer ou clarear a imagem\n"
                        << "Valores negativos para escurecer, valores positivos para clarear\n"
                        << "Os tons usados na imagem vão de 0 (preto) a 255 (branco)\n"
                        << "Mudança desejada: ";
                std::cin >> shading;
                shade(input_image, output_image, lines, columns, shading, &message);
                saveChanges(input_image, output_image, lines, columns);
                std::cout << "\nImagem " << message << " com sucesso.";
                break;

            case 6:
                edited = true;
                int orientation;
                std::cout << "\nEscolha qual orientação inverter\n"
                        << "0-Inverter horizontalmente\n"
                        << "1-Inverter verticalmente\n"
                        << "Opção: ";
                std::cin >> orientation;
                if(flip(input_image, output_image, lines, columns, orientation, &message) != 0){
                    std::cout << error;
                    return 1;
                }
                saveChanges(input_image, output_image, lines, columns);
                std::cout << "\nImagem invertida " << message << " com sucesso.";
                break;

            case 7:
                // Escrita do arquivo de saída da imagem.
                if(edited){
                    std::cout << "\nEntre com o nome da imagem de saída: ";
                    std::cin >> output_file;
                    output_file += ".pgm";
                    if(savePGM(output_file, output_image, lines, columns, tone) != 0){
                        std::cout << error;
                        return 1;
                    }
                    return 0;
                }else{
                    std::cout << "\nOperação cancelada.\n";
                    return 1;
                }
                break;

            default:
                std::cout << "\nErro: opção inválida.\n";
                return 1;
        }
    }while(option != 7);
    return 0;
}