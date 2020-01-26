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

void    push_face(t_data *d, uint32_t f[3])
{
    static int  capacity = 0;

    if (capacity == 0)
    {
        capacity = 64;
        if (!(d->faces = malloc(sizeof(uint32_t) * 3 * capacity)))
            exit(printf("malloc fail\n"));
    }
    if (d->nb_faces >= capacity)
    {
        capacity *= 2;
        if (!(d->faces = realloc(d->faces, sizeof(uint32_t) * 3 * capacity)))
            exit(printf("realloc fail\n"));
    }
    f[0]--;
    f[1]--;
    f[2]--;
    memcpy(&d->faces[d->nb_faces * 3], f, sizeof(uint32_t) * 3);
    d->nb_faces++;
}

void    parse_obj(t_data *d)
{
    FILE    *fp;
    char    line[100];
    float   v[3];
    int     f[3];

    fp = fopen("resources/teapot2.obj", "r");
    while(fgets(line, 100, fp))
    {
        if (sscanf(line, "v %f %f %f", v, &v[1], &v[2]) == 3)
            push_vertex(d, v);
        else if (sscanf(line, "f %d %d %d", f, &f[1], &f[2]) == 3)
            push_face(d, f);
    }
    fclose(fp);
    // for (int i = 0; i < d->nb_vertices; i++) {
    //     int j = i*3;
    //     printf("%f, %f, %f\n", d->vertices[j], d->vertices[j+1], d->vertices[j+2]);
    // }
    printf("%d vertices\n", d->nb_vertices);
    printf("%d faces\n", d->nb_faces);
}
