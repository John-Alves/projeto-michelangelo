#include <stdio.h>
#include <stdlib.h>

#define X_AXIS 1
#define Y_AXIS 0
#define RAISE_PEN 1
#define LOWER_PEN 0
#define MAX_STEPS 260  // Length of an axis in steps.

FILE *fp;
unsigned short position_x = 0, position_y = 0;  // Current position.
char penState;
char positioningMode = 1;  // Absolute mode = 1. Relative mode = 0.
float smallest, biggest;

void openGcodeFile(const char *fileName);
void interpretGcodeLine(char *lineCommand, unsigned short endOfLine);
float getLetterValue(char *lineCommand, unsigned short endOfLine, char character);
void instructionPen(char direction);
unsigned short map(float number, float inMin, float inMax, unsigned short outMin, unsigned short outMax);
void instructionStep(char axis, char direction, unsigned short steps);
void drawLine(unsigned short new_x, unsigned short new_y);

int main(){
    const char fileName[15];

    printf("Enter the file name: ");
    scanf("%s", &fileName);

    printf("Enter the smallest coordinate: ");
    scanf("%f", &smallest);

    printf("Enter the biggest coordinate: ");
    scanf("%f", &biggest);

    fp = fopen("instructions.txt", "w");

    if (fp != NULL){
        openGcodeFile(fileName);
    }

    printf("File generated successfully.\n");
    fclose(fp);

    return 0;
}

/**
  *  @brief  Opens the file in the G-ode and sends the lines individually to the interpreter.
  *  @param  fileName: Vector of characters containing the file name.
  *  @retval None.
  */
void openGcodeFile(const char *fileName){
	FILE *fp;

	fp = fopen(fileName, "r");

	if (fp != NULL){  // Try to open the archive.
        char character = fgetc(fp);
		char isComment = 0;  // Avoid extracting comments for the command line.
		char lineCommand[200];  // Will storage the current G-code line extracted from the file.
		unsigned short i = 0;

		while (character != EOF){
            if (character == '(')
                isComment = 1;

            if (!isComment){
                lineCommand[i] = character;
                i++;
            }

            if (character == ')')
                isComment = 0;


            // End of line, this means the full line command is on line_command variable.
            else if (character == '\n'){
                lineCommand[i - 1] = '\0';
                interpretGcodeLine(lineCommand, i);
                i = 0;
            }

			character = fgetc(fp);
		}
	}

	fclose(fp);
}

/**
  *  @brief  Adapts and sends the motion coordinates for the motors, besides regulating the position of the pen.
  *  @param  lineCommand: Line containing the instructions to be interpreted.
  *  @param  endOfLine: Number of characters in the instructions line.
  *  @retval None.
  */
void interpretGcodeLine(char *lineCommand, unsigned short endOfLine){
	short number;
	unsigned short auxiliary_x, auxiliary_y;
	float new_x, new_y;

	// Looking for commands stating with Z and get the number immediately after.
	number = getLetterValue(lineCommand, endOfLine, 'Z');

	switch(number){
	    case -1:
            instructionPen(LOWER_PEN);
            break;

        case 5:
            instructionPen(RAISE_PEN);
            break;

        default:
            break;
	}

    // Looking for commands stating with G and get the number immediately after.
    number = getLetterValue(lineCommand, endOfLine, 'G');

	switch(number){
		case 0:  // G00 -> Draws a line quickly.
		    new_x = getLetterValue(lineCommand, endOfLine, 'X');
			new_y = getLetterValue(lineCommand, endOfLine, 'Y');

            if (new_x != -2 && new_y != -2){
                auxiliary_x = map(new_x, smallest, biggest, 0, MAX_STEPS);
                auxiliary_y = map(new_y, smallest, biggest, 0, MAX_STEPS);

                (positioningMode)? drawLine(auxiliary_x, auxiliary_y) :
                                   drawLine(auxiliary_x + position_x, auxiliary_y + position_y);
            }

			break;

		case 1:  // G01 -> Draws a line at a controlled speed.
			new_x = getLetterValue(lineCommand, endOfLine, 'X');
			new_y = getLetterValue(lineCommand, endOfLine, 'Y');

			if (new_x != -2 && new_y != -2){
                auxiliary_x = map(new_x, smallest, biggest, 0, MAX_STEPS);
                auxiliary_y = map(new_y, smallest, biggest, 0, MAX_STEPS);

                 //  Note: The G01 command has a parameter to specify the movement speed, but it
                 //  will not be used in this project.

                (positioningMode)? drawLine(auxiliary_x, auxiliary_y) :
                                   drawLine(auxiliary_x + position_x, auxiliary_y + position_y);
            }

			break;

		case 90:  // Go to absolute mode.
			positioningMode = 1;
			break;

		case 91:  // Go to relative mode.
			positioningMode = 0;
			break;

		default:
		    break;
	}
}

/**
  *  @brief  Extracts the value immediately after the character.
  *  @param  lineCommand: Line containing the instructions to be interpreted.
  *  @param  endOfLine: Number of characters in the instructions line.
  *  @param  character: Character that has the value to be extracted.
  *  @retval If it exists, the extracted value; otherwise, it returns -2.
  */
float getLetterValue(char *lineCommand, unsigned short endOfLine, char character){
    unsigned short i;

	for (i = 0; i < endOfLine; i++){  // Looking for the letter in this line.
		if (lineCommand[i] == character){
            i++;

			char number[15];
			unsigned short j = 0;

			while (lineCommand[i + j] != ' ' && j < 15){  // Extract the number.
				number[j] = lineCommand[i + j];
				j++;
			}

			return atof(number);  // Convert the number to float.
		}
	}

	return -2;
}

/**
  *  @brief  Writes an instruction to the pen in the file.
  *  @param  direction: If true, the pen will be raised; if false, lowered.
  *  @retval None.
  */
void instructionPen(char direction){
    if ((direction) && !(penState)){
        penState = RAISE_PEN;
        fprintf(fp, "%s", "P1\n");
    }

    else if (!(direction) && (penState)){
        penState = LOWER_PEN;
        fprintf(fp, "%s", "P0\n");
    }
}

/**
  *  @brief  Maps one range to another.
  *  @param  number: Number to be mapped.
  *  @param  inMin: Smallest value of the source range.
  *  @param  inMax: Biggest value of the source range.
  *  @param  outMin: Smallest value of the target range.
  *  @param  outMax: Biggest value of the target range.
  *  @retval The number mapped.
  */
unsigned short map(float number, float inMin, float inMax, unsigned short outMin, unsigned short outMax){
    return (number - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

/**
  *  @brief  Writes an instruction to the stepper motor in the file.
  *  @param  axis: The engine to be active.
  *  @param  direction: The direction of motor rotation.
  *  @param  steps: The number of steps.
  *  @retval None.
  */
void instructionStep(char axis, char direction, unsigned short steps){
    (axis)? fprintf(fp, "%s", "X") : fprintf(fp, "%s", "Y");
    (direction)? fprintf(fp, "%s", "0 ") : fprintf(fp, "%s", "1 ");

    fprintf(fp, "%u\n", steps);

}

/**
  *  @brief  Draws a line.
  *  @param  new_x: X-axis coordinate.
  *  @param  new_y: Y-axis coordinate.
  *  @retval None.
  */
void drawLine(unsigned short new_x, unsigned short new_y){
    unsigned short i;
    short delta_x = new_x - position_x;  // Calculate x-variation.
    short delta_y = new_y - position_y;  // Calculate y-variation.
    char direction_x = (delta_x > 0)? 1 : 0;  // Direction of x-axis stepper motor.
    char direction_y = (delta_y > 0)? 1 : 0;  // Direction of y-axis stepper motor.
    float over = 0;

    if (!direction_x)
        delta_x = abs(delta_x);

    if (!direction_y)
        delta_y = abs(delta_y);

    if (delta_x == delta_y){
        for (i = 0; i < delta_x; i++){
            instructionStep(X_AXIS, direction_x, 1);
            instructionStep(Y_AXIS, direction_y, 1);
        }
    }

    else if (delta_x > delta_y){
        if (delta_y == 0){
            instructionStep(X_AXIS, direction_x, delta_x);
        }

        else {
            over = delta_x / 2;

            for (i = 0; i < delta_x; i++){
                over += delta_y;
                instructionStep(X_AXIS, direction_x, 1);

                if (over >= delta_x){
                    over -= delta_x;
                    instructionStep(Y_AXIS, direction_y, 1);
                }
            }
        }
    }

    else if (delta_x < delta_y){
        if (delta_x == 0){
            instructionStep(Y_AXIS, direction_y, delta_y);
        }

        else {
            over = delta_y / 2;

            for (i = 0; i < delta_y; i++){
                over += delta_x;
                instructionStep(Y_AXIS, direction_y, 1);

                if (over >= delta_y){
                    over -= delta_y;
                    instructionStep(X_AXIS, direction_x, 1);
                }
            }
        }
    }

    position_x = new_x;  // Update current x-position.
    position_y = new_y;  // Update current y-position.
}
