#include "scop.h"

void    push_vertex(t_data *d, float v[3])
{
    static int  capacity = 0;

    if (capacity == 0)
    {
        capacity = 64;
        if (!(d->vertices = malloc(sizeof(float) * 3 * capacity)))
            exit(printf("malloc fail\n"));
    }
    if (d->nb_vertices >= capacity)
    {
        capacity *= 2;
        if (!(d->vertices = realloc(d->vertices, sizeof(float) * 3 * capacity)))
            exit(printf("realloc fail\n"));
    }
    memcpy(&d->vertices[d->nb_vertices * 3], v, sizeof(float) * 3);
    d->nb_vertices++;
}

void    parse_obj(t_data *d)
{
    FILE    *fp;
    char    line[100];
    float   v[3];

    fp = fopen("resources/teapot2.obj", "r");
    while(fgets(line, 100, fp))
    {
        if (sscanf(line, "v %f %f %f", v, &v[1], &v[2]) == 3)
            push_vertex(d, v);
    }
    fclose(fp);
    // for (int i = 0; i < d->nb_vertices; i++) {
    //     int j = i*3;
    //     printf("%f, %f, %f\n", d->vertices[j], d->vertices[j+1], d->vertices[j+2]);
    // }
    printf("%d\n", d->nb_vertices);
}
