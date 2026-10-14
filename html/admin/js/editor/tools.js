/** Применение инструментов + линия Брезенхэма */

import { state } from '../state.js';

/** Внутри ли клетка комнаты */
export function isInsideRoom(room, x, y) {
  return x >= 0 && x < room.width && y >= 0 && y < room.height;
}

/** Применить инструмент к одной клетке. Возвращает true, если что-то изменилось. */
export function applyToolToCell(room, x, y) {
  if (!isInsideRoom(room, x, y)) return false;
  const current = room.cells[y][x];
  const next = state.currentTool === 'wall' ? 1 : 0;
  if (current === next) return false;
  room.cells[y][x] = next;
  return true;
}

/** Линия Брезенхэма между двумя клетками (включая обе) */
export function lineBetween(x0, y0, x1, y1) {
  const points = [];
  let dx = Math.abs(x1 - x0);
  let dy = Math.abs(y1 - y0);
  let sx = x0 < x1 ? 1 : -1;
  let sy = y0 < y1 ? 1 : -1;
  let err = dx - dy;
  while (true) {
    points.push({ x: x0, y: y0 });
    if (x0 === x1 && y0 === y1) break;
    const e2 = 2 * err;
    if (e2 > -dy) { err -= dy; x0 += sx; }
    if (e2 < dx) { err += dx; y0 += sy; }
  }
  return points;
}