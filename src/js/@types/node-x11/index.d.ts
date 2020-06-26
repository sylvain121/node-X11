declare module 'node-x11' {
    enum MOUSE_BUTTON {
        LEFT = 1,
        MIDDLE = 2,
        RIGHT = 3
    }

    export class XImage {
        readonly width: number;
        readonly height: number;
        readonly depth: number;
        readonly bits_per_pixel: number;
        readonly bytes_per_line: number;
        readonly data: Buffer;
    }
    type GetImageCallback = (err: Error, image: Ximage) => void;
    export function init(display?: string): { width: number, height: number, depth: number };

    export function getImageSync(xoffset?: number, yoffset?: number, width?: number, height?: number): XImage;

    export function getImage(xoffset?:number, yoffset?: number, width?: number, height?: number, callback?: GetImageCallback);

    export function keyPressWithKeycode(keyCode: number, isDown: boolean);

    export function keyPressWithKeysym(KeySym: number, isDown: boolean);

    export function mouseMove(xPosition: number, yPosition: number);

    export function mouseButton(button: number, isDown: boolean);

    export function close();

}
