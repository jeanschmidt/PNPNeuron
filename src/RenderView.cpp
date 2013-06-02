#include "RenderView.h"
#include "Standarts.h"
#include "stdio.h"

#include <mgl2/mgl.h>

void RenderView::renderArea(void) {
    if(universo == NULL) {
        printf("No universe to copile, quiting...\n");
        return;
    }

    mglData dat(RESOLUTION+20, RESOLUTION+20, RESOLUTION+20);

    universo->rewind();
    Particula *particula = universo->getParticula();
    Posicao *posicao = universo->getPosicao();

    printf("*Loading and preparing area data..\n");

    while( particula && posicao ) {
        dat.a[ (posicao->x+10) + (RESOLUTION+20)*((posicao->y+10) + (RESOLUTION+20)*(posicao->z+10)) ] = 1.0f;
        universo->next();
        particula = universo->getParticula();
        posicao = universo->getPosicao();
    }

    printf("*Rendering area...\n");
    mglGraph *gr = new mglGraph(0,800,800);
    gr->Title("Neural Model");
    gr->Rotate(60,40);  
    gr->Box(); 
    gr->Alpha(true); 
    gr->Surf3(dat);
//    gr->Cloud(dat, "wyrRk");
    gr->WritePNG("results/area.png");

    delete gr;
}

void RenderView::renderMembrane(void) {
    if(universo == NULL) {
        printf("No universe to render, quitting...\n");
        return;
    }

    mglData dat(RESOLUTION+20, RESOLUTION+20, RESOLUTION+20);

    universo->rewind();
    Particula *particula = universo->getParticula();
    Posicao *posicao = universo->getPosicao();

    while( particula && posicao ) {
        dat.a[ (posicao->x+10) + (RESOLUTION+20)*((posicao->y+10) + (RESOLUTION+20)*(posicao->z+10)) ] = (particula->paredeCelular.size() ? 1.0f : 0.0f);
        universo->next();
        particula = universo->getParticula();
        posicao = universo->getPosicao();
    }

    printf("*Rendering Membrane...\n");

    mglGraph *gr = new mglGraph(0,800,800);
    gr->Title("Celular Membrane");
    gr->Rotate(60,40);
    gr->Box();
    gr->Alpha(true);
    gr->Surf3(dat);
    gr->WritePNG("results/membrana.png");

    delete gr;
}

void RenderView::renderEPotential(unsigned frame) {
    if(universo == NULL) {
        printf("No universe to render, quitting...\n");
        return;
    }

    printf("*Preparing to render eletric potencial %d\n", frame);
    mglData dat(RESOLUTION+20, RESOLUTION+20, RESOLUTION+20);

    universo->rewind();

    while(true) {
        Particula *particula = universo->getParticula();
        Posicao *posicao = universo->getPosicao();

        if(!particula || !posicao)
            break;

        if(particula->meio != &(StandartElements::citoplasma)) {
            universo->next();
            continue;
        }

        float potencial_ponto = 0.0f;
        unsigned media = 0;

        if(particula->paredeCelularSuperficies[0]) {
            Posicao p(*posicao);
            p.x++;
            p.updateFastCompare(RESOLUTION);
            Particula *visinho = universo->getPosicao(p);
            if(visinho) {
                potencial_ponto += visinho->eletric_potential;
                media++;
            }
        }
        if(particula->paredeCelularSuperficies[1]) {
            Posicao p(*posicao);
            p.x--;
            p.updateFastCompare(RESOLUTION);
            Particula *visinho = universo->getPosicao(p);
            if(visinho) {
                potencial_ponto += visinho->eletric_potential;
                media++;
            }
        }
        if(particula->paredeCelularSuperficies[2]) {
            Posicao p(*posicao);
            p.y++;
            p.updateFastCompare(RESOLUTION);
            Particula *visinho = universo->getPosicao(p);
            if(visinho) {
                potencial_ponto += visinho->eletric_potential;
                media++;
            }
        }
        if(particula->paredeCelularSuperficies[3]) {
            Posicao p(*posicao);
            p.y--;
            p.updateFastCompare(RESOLUTION);
            Particula *visinho = universo->getPosicao(p);
            if(visinho) {
                potencial_ponto += visinho->eletric_potential;
                media++;
            }
        }
        if(particula->paredeCelularSuperficies[4]) {
            Posicao p(*posicao);
            p.z++;
            p.updateFastCompare(RESOLUTION);
            Particula *visinho = universo->getPosicao(p);
            if(visinho) {
                potencial_ponto += visinho->eletric_potential;
                media++;
            }
        }
        if(particula->paredeCelularSuperficies[5]) {
            Posicao p(*posicao);
            p.z--;
            p.updateFastCompare(RESOLUTION);
            Particula *visinho = universo->getPosicao(p);
            if(visinho) {
                potencial_ponto += visinho->eletric_potential;
                media++;
            }
        }

        if(potencial_ponto > 0.0f) {
            potencial_ponto /= media;
            dat.a[ (posicao->x+10) + (RESOLUTION+20)*((posicao->y+10) + (RESOLUTION+20)*(posicao->z+10)) ] = (particula->eletric_potential - potencial_ponto)*100000000; //os mV sao muito pequenos para serem exibidos...
        }
      
        universo->next();
    }

    printf("*Rendering Eletric Potential %d\n", frame);
    char name[128];
    snprintf(name, 128, "results/epotencial_%d.png", frame);

    mglGraph *gr = new mglGraph(0, 800, 800);
    gr->Title("Eletric Potential");
    gr->Rotate(60,40);
    gr->Box(); 
    gr->Alpha(true);
//http://mathgl.sourceforge.net/doc_en/doc_en_106.html#Color-styles
//    gr->Cloud(dat, "wyrRk");
    gr->Cloud(dat, "rywgb");
    gr->WritePNG(name);
    delete gr;
}


