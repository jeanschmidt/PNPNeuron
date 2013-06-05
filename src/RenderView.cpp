#include <stdio.h>
#include <mgl2/mgl.h>

#include "RenderView.h"
#include "Standarts.h"
#include "Config.h"

void RenderView::renderArea(void) {
    if(universo == NULL) {
        fprintf(stderr,"No universe to copile, quiting...\n");
        return;
    }

    mglData dat(RESOLUTION+20, RESOLUTION+20, RESOLUTION+20);

    universo->rewind();
    Particula *particula = universo->getParticula();
    Posicao *posicao = universo->getPosicao();

    fprintf(stderr,"*Loading and preparing area data..\n");

    while( particula && posicao ) {
        dat.a[ (posicao->x+10) + (RESOLUTION+20)*((posicao->y+10) + (RESOLUTION+20)*(posicao->z+10)) ] = 1.0f;
        universo->next();
        particula = universo->getParticula();
        posicao = universo->getPosicao();
    }

    ::std::string path(defaultDirectory);
    path += "/";
    path += neuralModelFile;
    path += ".png";

    fprintf(stderr,"*Rendering area...\n");
    mglGraph *gr = new mglGraph(0,res_x,res_y);
    gr->Title(neuralModelTitle);

    if(angle_3d) {
        gr->SubPlot(2,1,0);
        gr->Rotate(rotate_x,rotate_y+angle_3d);
        gr->Box();
        gr->Alpha(true);
        gr->Surf3(dat);

        gr->SubPlot(2,1,1);
        gr->Rotate(rotate_x,rotate_y-angle_3d);
        gr->Box();
        gr->Alpha(true);
        gr->Surf3(dat);
    } else {
        gr->Rotate(rotate_x,rotate_y);  
        gr->Box(); 
        gr->Alpha(true); 
        gr->Surf3(dat);
    }
    gr->WritePNG(path.c_str());

    delete gr;
}

void RenderView::renderMembrane(void) {
    if(universo == NULL) {
        fprintf(stderr,"No universe to render, quitting...\n");
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

    fprintf(stderr,"*Rendering Membrane...\n");

    ::std::string path(defaultDirectory);
    path += "/";
    path += neuralMembraneFile;
    path += ".png";

    mglGraph *gr = new mglGraph(0,res_x,res_y);
    gr->Title(neuralMembraneTitle);

    if(angle_3d) {
        gr->SubPlot(2,1,0);
        gr->Rotate(rotate_x,rotate_y+angle_3d);
        gr->Box();
        gr->Alpha(true);
        gr->Surf3(dat);

        gr->SubPlot(2,1,1);
        gr->Rotate(rotate_x,rotate_y-angle_3d);
        gr->Box();
        gr->Alpha(true);
        gr->Surf3(dat);
    } else {
        gr->Rotate(rotate_x,rotate_y);  
        gr->Box(); 
        gr->Alpha(true); 
        gr->Surf3(dat);
    }

    gr->WritePNG(path.c_str());

    delete gr;
}

void RenderView::renderEPotential(unsigned frame) {
    if(universo == NULL) {
        fprintf(stderr,"No universe to render, quitting...\n");
        return;
    }

    fprintf(stderr,"*Preparing to render eletric potencial %d\n", frame);
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

    fprintf(stderr,"*Rendering Eletric Potential %d\n", frame);
    char name[6];
    snprintf(name, sizeof(name), "_%03d", frame);

    ::std::string path(defaultDirectory);
    path += "/";
    path += neuralEletricFile;
    path += name;
    path += ".png";

    mglGraph *gr = new mglGraph(0, res_x, res_y);
    gr->Title(neuralEletricTitle);
    gr->Rotate(rotate_x,rotate_y);
    if(angle_3d) {
        gr->SubPlot(2,1,0);
        gr->Rotate(rotate_x,rotate_y+angle_3d);
        gr->Box();
        gr->Alpha(true);
        gr->Cloud(dat, colorPattern);

        gr->SubPlot(2,1,1);
        gr->Rotate(rotate_x,rotate_y-angle_3d);
        gr->Box();
        gr->Alpha(true);
        gr->Cloud(dat, colorPattern);
    } else {
        gr->Rotate(rotate_x,rotate_y);  
        gr->Box(); 
        gr->Alpha(true); 
        gr->Cloud(dat, colorPattern);
    }
    gr->WritePNG(path.c_str());
    delete gr;
}

void RenderView::init(void) {
    neuralModelTitle = "Neural Model";
    neuralMembraneTitle = "Celular Membrane";
    neuralEletricTitle = "Eletric Potential";
    neuralModelFile = "area";
    neuralMembraneFile = "membrane";
    neuralEletricFile = "eletric_potential";
    defaultDirectory = "results";
    colorPattern = "rywgb";
    angle_3d = 0;

    const ConfigList::Config *c;

    c = ConfigHolder::get()->get("out_color_pattern");
    if(c) colorPattern = c->value;

    c = ConfigHolder::get()->get("out_res_x");
    if(c) res_x = atoi(c->value);

    c = ConfigHolder::get()->get("out_res_y");
    if(c) res_y = atoi(c->value);

    c = ConfigHolder::get()->get("out_rotate_x");
    if(c) rotate_x = atoi(c->value);

    c = ConfigHolder::get()->get("out_rotate_y");
    if(c) rotate_y = atoi(c->value);

    c = ConfigHolder::get()->get("out_neuralModelTitle");
    if(c) neuralModelTitle = c->value;

    c = ConfigHolder::get()->get("out_neuralMembraneTile");
    if(c) neuralMembraneTitle = c->value;

    c = ConfigHolder::get()->get("out_neuralEletricTile");
    if(c) neuralEletricTitle = c->value;

    c = ConfigHolder::get()->get("out_neuralModel_file");
    if(c) neuralEletricFile = c->value;

    c = ConfigHolder::get()->get("out_neuralMembrane_file");
    if(c) neuralMembraneFile = c->value;

    c = ConfigHolder::get()->get("out_neuralEletric_file");
    if(c) neuralEletricFile = c->value;

    c = ConfigHolder::get()->get("out_directory");
    if(c) defaultDirectory = c->value;

    c = ConfigHolder::get()->get("out_3d_enable");
    if(c && strcmp(c->value, "true")==0) {
        c = ConfigHolder::get()->get("out_3d_angle");
        if(c) angle_3d = atol(c->value);
    }
}

