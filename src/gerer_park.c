#include <stdio.h>
#include <string.h>
#include "gerer_park.h"

// Utility function to make sure that there are no spaces in strings.
void replace_spaces_with_underscores(char *str) {
    int i, j;
    for (i = 0, j = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ') {
            str[j++] = str[i];
        } else {
            str[j++] = '_';
        }
    }
    str[j] = '\0'; // Null-terminate the modified string
}

// Add a new parking record to the file
int ajouter_park(const char *filename, Parking p) {
    FILE *f = fopen(filename, "a");
    FILE *rf = fopen(filename, "r");
    
    // Increment the id.
    int id_increment = 0;
    if (rf != NULL)
    {
        Parking sp;
        while (fscanf(rf, "%d %49s %99s %19s %d %d %d %d %d %d\n",
                      &sp.id, sp.name, sp.location, sp.type, &sp.capacity,
                      &sp.is_24_hours, &sp.open_hour, &sp.close_hour, &sp.hourly_rate,
                      &sp.agent) != EOF) {
            if (sp.id > id_increment)
            {
                id_increment = sp.id;
            }
        }
        id_increment += 1;

        fclose(rf);
    }

    if (f != NULL) {
        replace_spaces_with_underscores(p.name);
        replace_spaces_with_underscores(p.location);
        fprintf(f, "%d %s %s %s %d %d %d %d %d %d\n",
                id_increment, p.name, p.location, p.type, p.capacity,
                p.is_24_hours, p.open_hour, p.close_hour, p.hourly_rate,
                p.agent);
        fclose(f);
        return 1;
    }
    return 0;
}

// Modify an existing parking record based on ID
int modifier_park(const char *filename, int id, Parking new_p) {
    int found = 0;
    Parking p;
    FILE *f = fopen(filename, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (f == NULL || temp == NULL) {
        return 0;
    }

    while (fscanf(f, "%d %49s %99s %19s %d %d %d %d %d %d\n",
                  &p.id, p.name, p.location, p.type, &p.capacity,
                  &p.is_24_hours, &p.open_hour, &p.close_hour, &p.hourly_rate,
                  &p.agent) != EOF) {
        if (p.id == id) {
            replace_spaces_with_underscores(p.name);
            replace_spaces_with_underscores(p.location);
            fprintf(temp, "%d %s %s %s %d %d %d %d %d %d\n",
                    new_p.id, new_p.name, new_p.location, new_p.type,
                    new_p.capacity, new_p.is_24_hours, new_p.open_hour,
                    new_p.close_hour, new_p.hourly_rate, new_p.agent);
            found = 1;
        } else {
            fprintf(temp, "%d %s %s %s %d %d %d %d %d %d\n",
                    p.id, p.name, p.location, p.type, p.capacity,
                    p.is_24_hours, p.open_hour, p.close_hour, p.hourly_rate,
                    p.agent);
        }
    }
    fclose(f);
    fclose(temp);

    remove(filename);
    rename("temp.txt", filename);

    return found;
}

// Delete a parking record based on ID
int supprimer_park(const char *filename, int id) {
    int found = 0;
    Parking p;
    FILE *f = fopen(filename, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (f == NULL || temp == NULL) {
        return 0;
    }

    while (fscanf(f, "%d %49s %99s %19s %d %d %d %d %d %d\n",
                  &p.id, p.name, p.location, p.type, &p.capacity,
                  &p.is_24_hours, &p.open_hour, &p.close_hour, &p.hourly_rate,
                  &p.agent) != EOF) {
        if (p.id != id) {
            fprintf(temp, "%d %s %s %s %d %d %d %d %d %d\n",
                    p.id, p.name, p.location, p.type, p.capacity,
                    p.is_24_hours, p.open_hour, p.close_hour, p.hourly_rate,
                    p.agent);
        } else {
            found = 1;
        }
    }
    fclose(f);
    fclose(temp);

    remove(filename);
    rename("temp.txt", filename);

    return found;
}

// Search for a parking record by ID
Parking chercher_park(const char *filename, int id) {
    Parking p;
    p.id = -1; // Default to -1 if not found
    FILE *f = fopen(filename, "r");
    if (f != NULL) {
        while (fscanf(f, "%d %49s %99s %19s %d %d %d %d %d %d\n",
                      &p.id, p.name, p.location, p.type, &p.capacity,
                      &p.is_24_hours, &p.open_hour, &p.close_hour, &p.hourly_rate,
                      &p.agent) != EOF) {
            if (p.id == id) {
                fclose(f);
                return p;
            }
        }
        fclose(f);
    }
    p.id = -1; // Not found
    return p;
}

int attribuer_agent(const char* filename, int agent_id, int park_id)
{
    if (agent_id >= 0 && park_id >= 0)
    {
        Parking p = chercher_park(filename, park_id);
        if (p.id != -1)
        {
            p.agent = agent_id;
            if (modifier_park(filename, park_id, p) == 1)
            {
                return 1;
            }
        }

        return 0;
    }

    return 0;
}