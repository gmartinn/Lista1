#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 100
#define MAX_GENRE_LENGTH 50
#define MAX_LOCATION_LENGTH 100
#define MAX_ALBUMS 10
#define MAX_ARTISTS 100

typedef struct {
    char nome[MAX_NAME_LENGTH];
    char genero[MAX_GENRE_LENGTH];
    char local[MAX_LOCATION_LENGTH];
    char albums[MAX_ALBUMS][MAX_NAME_LENGTH];
    int numAlbums;
} Artista;

void printMenu() {
    printf("\nSelecione uma opcao:\n");
    printf("1. Insercao ordenada de novos artistas\n");
    printf("2. Remocao de um artista\n");
    printf("3. Edicao de um artista\n");
    printf("4. Busca binaria por um artista\n");
    printf("5. Busca sequencial por um album\n");
    printf("0. Sair\n");
    printf("Opcao: ");
}

void readArtistsFromFile(Artista *artistas, int *numArtistas) {
    FILE *arquivo = fopen("artistas.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[MAX_NAME_LENGTH];
    int i = 0;
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (strcmp(linha, "==========\n") == 0) {
            i++;
            continue;
        }

        linha[strcspn(linha, "\n")] = '\0'; // Remover o caractere de nova linha

        if (i >= MAX_ARTISTS) {
            printf("Limite de artistas excedido. Alguns artistas foram ignorados.\n");
            break;
        }

        switch (i % 4) {
            case 0:
                strcpy(artistas[i/4].nome, linha);
                break;
            case 1:
                strcpy(artistas[i/4].genero, linha);
                break;
            case 2:
                strcpy(artistas[i/4].local, linha);
                break;
            case 3:
                strcpy(artistas[i/4].albums[artistas[i/4].numAlbums], linha);
                artistas[i/4].numAlbums++;
                break;
        }
    }

    *numArtistas = i / 4;

    fclose(arquivo);
}

void saveArtistsToFile(Artista *artistas, int numArtistas) {
    FILE *arquivo = fopen("artistas.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    for (int i = 0; i < numArtistas; i++) {
        fprintf(arquivo, "%s\n", artistas[i].nome);
        fprintf(arquivo, "%s\n", artistas[i].genero);
        fprintf(arquivo, "%s\n", artistas[i].local);
        for (int j = 0; j < artistas[i].numAlbums; j++) {
            fprintf(arquivo, "%s\n", artistas[i].albums[j]);
        }
        fprintf(arquivo, "==========\n");
    }

    fclose(arquivo);
}

void insertArtist(Artista *artistas, int *numArtistas) {
    if (*numArtistas >= MAX_ARTISTS) {
        printf("Limite de artistas atingido.\n");
        return;
    }

    Artista novoArtista;
    printf("Nome do artista: ");
    fgets(novoArtista.nome, sizeof(novoArtista.nome), stdin);
    novoArtista.nome[strcspn(novoArtista.nome, "\n")] = '\0';

    printf("Genero musical: ");
    fgets(novoArtista.genero, sizeof(novoArtista.genero), stdin);
    novoArtista.genero[strcspn(novoArtista.genero, "\n")] = '\0';

    printf("Local de criacao/nascimento: ");
    fgets(novoArtista.local, sizeof(novoArtista.local), stdin);
    novoArtista.local[strcspn(novoArtista.local, "\n")] = '\0';

    printf("Albums (digite 'fim' para finalizar):\n");
    char album[MAX_NAME_LENGTH];
    while (1) {
        fgets(album, sizeof(album), stdin);
        album[strcspn(album, "\n")] = '\0';
        if (strcmp(album, "fim") == 0) {
            break;
        }
        strcpy(novoArtista.albums[novoArtista.numAlbums], album);
        novoArtista.numAlbums++;
    }

    // Inserir o novo artista na posição correta (ordem alfabética)
    int posicao = *numArtistas;
    while (posicao > 0 && strcmp(artistas[posicao - 1].nome, novoArtista.nome) > 0) {
        artistas[posicao] = artistas[posicao - 1];
        posicao--;
    }
    artistas[posicao] = novoArtista;
    (*numArtistas)++;

    // Salvar as alterações no arquivo
    saveArtistsToFile(artistas, *numArtistas);

    printf("Artista inserido com sucesso!\n");
}

void removeArtist(Artista *artistas, int *numArtistas) {
    if (*numArtistas == 0) {
        printf("Nenhum artista cadastrado.\n");
        return;
    }

    char nome[MAX_NAME_LENGTH];
    printf("Digite o nome do artista que deseja remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    int posicao = -1;
    for (int i = 0; i < *numArtistas; i++) {
        if (strcmp(artistas[i].nome, nome) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao == -1) {
        printf("Artista nao encontrado.\n");
        return;
    }

    // Remover o artista da lista
    for (int i = posicao; i < *numArtistas - 1; i++) {
        artistas[i] = artistas[i + 1];
    }
    (*numArtistas)--;

    // Salvar as alterações no arquivo
    saveArtistsToFile(artistas, *numArtistas);

    printf("Artista removido com sucesso!\n");
}

void editArtist(Artista *artistas, int numArtistas) {
    if (numArtistas == 0) {
        printf("Nenhum artista cadastrado.\n");
        return;
    }

    char nome[MAX_NAME_LENGTH];
    printf("Digite o nome do artista que deseja editar: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    int posicao = -1;
    for (int i = 0; i < numArtistas; i++) {
        if (strcmp(artistas[i].nome, nome) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao == -1) {
        printf("Artista nao encontrado.\n");
        return;
    }

    Artista artista = artistas[posicao];

    printf("Novo nome do artista: ");
    fgets(artista.nome, sizeof(artista.nome), stdin);
    artista.nome[strcspn(artista.nome, "\n")] = '\0';

    printf("Novo genero musical: ");
    fgets(artista.genero, sizeof(artista.genero), stdin);
    artista.genero[strcspn(artista.genero, "\n")] = '\0';

    printf("Novo local de criacao/nascimento: ");
    fgets(artista.local, sizeof(artista.local), stdin);
    artista.local[strcspn(artista.local, "\n")] = '\0';

    printf("Novos albums (digite 'fim' para finalizar):\n");
    char album[MAX_NAME_LENGTH];
    artista.numAlbums = 0;
    while (1) {
        fgets(album, sizeof(album), stdin);
        album[strcspn(album, "\n")] = '\0';
        if (strcmp(album, "fim") == 0) {
            break;
        }
        strcpy(artista.albums[artista.numAlbums], album);
        artista.numAlbums++;
    }

    artistas[posicao] = artista;

    // Salvar as alterações no arquivo
    saveArtistsToFile(artistas, numArtistas);

    printf("Artista editado com sucesso!\n");
}

void binarySearchArtist(Artista *artistas, int numArtistas) {
    if (numArtistas == 0) {
        printf("Nenhum artista cadastrado.\n");
        return;
    }

    char nome[MAX_NAME_LENGTH];
    printf("Digite o nome do artista que deseja buscar: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    int inicio = 0;
    int fim = numArtistas - 1;
    int encontrado = 0;
     while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        if (strcmp(artistas[meio].nome, nome) == 0) {
            encontrado = 1;
            printf("Artista encontrado:\n");
            printf("Nome: %s\n", artistas[meio].nome);
            printf("Genero: %s\n", artistas[meio].genero);
            printf("Local de criacao/nascimento: %s\n", artistas[meio].local);
            printf("Albums:\n");
            for (int i = 0; i < artistas[meio].numAlbums; i++) {
                printf("- %s\n", artistas[meio].albums[i]);
            }
            break;
        } else if (strcmp(artistas[meio].nome, nome) < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    if (!encontrado) {
        printf("Artista nao encontrado.\n");
    }
}

void sequentialSearchAlbum(Artista *artistas, int numArtistas) {
    if (numArtistas == 0) {
        printf("Nenhum artista cadastrado.\n");
        return;
    }

    char album[MAX_NAME_LENGTH];
    printf("Digite o nome do album que deseja buscar: ");
    fgets(album, sizeof(album), stdin);
    album[strcspn(album, "\n")] = '\0';

    int encontrado = 0;
    for (int i = 0; i < numArtistas; i++) {
        for (int j = 0; j < artistas[i].numAlbums; j++) {
            if (strcmp(artistas[i].albums[j], album) == 0) {
                encontrado = 1;
                printf("Album encontrado:\n");
                printf("Nome do artista: %s\n", artistas[i].nome);
                printf("Genero: %s\n", artistas[i].genero);
                printf("Local de criacao/nascimento: %s\n", artistas[i].local);
                printf("Album: %s\n", artistas[i].albums[j]);
                break;
            }
        }
    }

    if (!encontrado) {
        printf("Album nao encontrado.\n");
    }
}

int main() {
    Artista artistas[MAX_ARTISTS];
    int numArtistas = 0;

    readArtistsFromFile(artistas, &numArtistas);

    int opcao;
    do {
        printMenu();
        scanf("%d", &opcao);
        getchar(); // Limpar o caractere de nova linha no buffer

        switch (opcao) {
            case 1:
                insertArtist(artistas, &numArtistas);
                break;
            case 2:
                removeArtist(artistas, &numArtistas);
                break;
            case 3:
                editArtist(artistas, numArtistas);
                break;
            case 4:
                binarySearchArtist(artistas, numArtistas);
                break;
            case 5:
                sequentialSearchAlbum(artistas, numArtistas);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}