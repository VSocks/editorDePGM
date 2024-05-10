typedef int tImage[1000][1000];

std::string error;

// Cria matriz "imagem input" com os valores do arquivo de entrada
// esse arquivo será então processado para formar a matriz "imagem output"
int loadPGM(std::string name, tImage iImg, int *lin, int *col, int *tone){
    std::string type;
    std::ifstream file(name);

    if(!file.is_open()){
        error = "\nErro: Arquivo não encontrado.\n";
        return 1;
    }

    file >> type;
    if(type != "P2"){
        error = "\nErro: Arquivo não tem formato P2.\n";
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
        error = "\nErro: Arquivo não encontrado.\n";
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

// Salva alterações na imagem de input toda vez que uma alteração for feita na imagem output.
// Assim é possível fazer várias alterações em sequência, uma vez que nas outras funções as
// mudanças da imagem de output usam a imagem input como base
void saveChanges(tImage iImg, tImage oImg, int lin, int col){
    for(int i = 0; i < lin; i++)
        for(int j = 0; j < col; j++)
            iImg[i][j] = oImg[i][j];
}

// Função que rotaciona os valores da matriz imagem input
// e coloca suas novas posições na matriz imagen output
int rotate(tImage iImg, tImage oImg, int *lin, int *col, int dir, std::string *msg){
    switch(dir){
        case 0: // Roda os valores da matriz 90 graus à esquerda
            *msg = "esquerda";
            for(int i = 0; i < *col; i++)
                for(int j = 0; j < *lin; j++)
                    oImg[i][j] = iImg[j][*col - i];
            break;

        case 1: // Roda os valores da matriz 90 graus à direita
            *msg = "direita";
            for(int i = 0; i < *col; i++)
                for(int j = 0; j < *lin; j++)
                    oImg[i][j] = iImg[*lin - j][i];
            break;

        default:
            error = "\nErro: Opção inválida.\n";
            return 1;
    }
    std::swap(*lin, *col);
    saveChanges(iImg, oImg, *lin, *col);
    return 0;
}

// Lê cada valor da imagem input e compara com o valor desejado pelo usuário
// para a binarização. Valores maiores são preenchidos por branco na imagem output
// enquanto valores menores são preenchidos com preto
int binarize(tImage iImg, tImage oImg, int lin, int col, int dsr_tone){
    if(dsr_tone > 255 || dsr_tone < 0){
        error = "\nErro: Valor inválido.\n";
        return 1;
    }
    else
        for(int i = 0; i < lin; i++)
            for(int j = 0; j < col; j++)
                if(iImg[i][j] < dsr_tone)
                    oImg[i][j] = 0;
                else
                    oImg[i][j] = 255;
    
    saveChanges(iImg, oImg, lin, col);
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
    saveChanges(iImg, oImg, *lin, *col);
}

// Função que aplica filtro passa-baixa
void smooth(tImage iImg, tImage oImg, int lin, int col){
    int matrix[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

    for(int i = 0; i < lin; i++)
        for(int j = 0; j < col; j++){
            int sum1 = 0;
            for(int k = -1; k <= 1; k++)
                for(int l = -1; l <= 1; l++)
                    sum1 += iImg[i + 1][j + l] * matrix[k + 1][l + 1];
            oImg[i][j] = sum1 / 9;
        }
    saveChanges(iImg, oImg, lin, col);
}

// Coloca valores invertidos da matriz imagem output na matriz imagem output
void negative(tImage iImg, tImage oImg, int lin, int col, int *tone){
    for(int i = 0; i < lin; i++)
        for(int j = 0; j < col; j++)
            oImg[i][j] = *tone - iImg[i][j];

    saveChanges(iImg, oImg, lin, col);
}

// Esclarese ou escurece a imagem de acordo com o que o usuário especifica
void shade(tImage iImg, tImage oImg, int lin, int col, int shade, std::string *msg){
    if(shade < 0){
        *msg = "escurecida";
        for(int i = 0; i < lin; i++)
            for(int j = 0; j < col; j++)
                if(iImg[i][j] + shade < 0)
                    oImg[i][j] = 0;
                else
                    oImg[i][j] = iImg[i][j] + shade;
    }else{
        *msg = "clareada";
        for(int i = 0; i < lin; i++)
            for(int j = 0; j < col; j++)
                if(iImg[i][j] + shade > 255)
                    oImg[i][j] = 255;
                else
                    oImg[i][j] = iImg[i][j] + shade;
    }
    saveChanges(iImg, oImg, lin, col);
}

// Inverte a orientação da imagem verticalmente ou horizontalmente
int flip(tImage iImg, tImage oImg, int lin, int col, int orien, std::string *msg){
    switch(orien){
        //inverter horizontal
        case 0:
            *msg = "horizontalmente";
            for(int i = 0; i < lin; i++)
                for(int j = 0; j < col; j++)
                    oImg[i][j] = iImg[i][col - j];
            break;
        //inverter vertical
        case 1:
            *msg = "verticalmente";
            for(int i = 0; i < lin; i++)
                for(int j = 0; j < col; j++)
                    oImg[i][j] = iImg[lin - i][j];
            break;
        default:
            error = "\nErro: Opção inválida\n";
            return 1;
    }
    saveChanges(iImg, oImg, lin, col);
    return 0;
}