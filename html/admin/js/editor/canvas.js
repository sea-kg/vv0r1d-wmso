/** Canvas: рендер, drag-рисование, координаты */

import { state } from '../state.js';
import { isInsideRoom, applyToolToCell, lineBetween } from './tools.js';
import { hexToRgba, $ } from '../utils.js';
import { updateStats, updateCellInfo } from '../ui/inspector.js';
import { renderRoomList } from '../ui/sidebar.js';

let canvas, ctx;
const coordHint = $('coordHint');

/** Инициализация canvas */
export function initCanvas() {
  canvas = $('roomCanvas');
  ctx = canvas.getContext('2d');

  canvas.addEventListener('pointerdown', onPointerDown);
  canvas.addEventListener('pointermove', onPointerMove);
  canvas.addEventListener('pointerup', onPointerUp);
  canvas.addEventListener('pointercancel', onPointerUp);
  canvas.addEventListener('pointerleave', onPointerLeave);
  canvas.addEventListener('contextmenu', (e) => e.preventDefault());

  // Страховка — если указатель отпущен вне canvas
  window.addEventListener('pointerup', onPointerUp);

  // Подписки на события состояния
  state.on('room:changed', () => redraw());
  state.on('room:select', () => redraw());
  state.on('room:resized', () => {
    const room = state.currentRoom();
    if (room) resizeCanvasToRoom(room);
  });
}

/** Подогнать размер canvas под комнату */
export function resizeCanvasToRoom(room) {
  const maxW = 900, maxH = 700;
  const cellSize = Math.max(
    10,
    Math.min(
      Math.floor(maxW / room.width),
      Math.floor(maxH / room.height),
      40
    )
  );
  canvas.width = room.width * cellSize;
  canvas.height = room.height * cellSize;
  canvas.dataset.cellSize = cellSize;
}

/** Полная перерисовка */
export function redraw() {
  const room = state.currentRoom();
  if (!room) {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    return;
  }
  drawRoom(room);
}

function drawRoom(room) {
  const cs = parseInt(canvas.dataset.cellSize) || 30;
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  for (let y = 0; y < room.height; y++) {
    for (let x = 0; x < room.width; x++) {
      const cell = room.cells[y][x];
      const px = x * cs, py = y * cs;

      if (cell === 1) {
        // Стена
        ctx.fillStyle = '#475569';
        ctx.fillRect(px, py, cs, cs);
        ctx.strokeStyle = '#1e293b';
        ctx.lineWidth = 1;
        ctx.strokeRect(px + 0.5, py + 0.5, cs - 1, cs - 1);
        if (cs >= 16) {
          ctx.fillStyle = 'rgba(255,255,255,0.06)';
          ctx.fillRect(px + 1, py + 1, cs - 2, Math.max(2, cs * 0.15));
        }
      } else {
        // Пол
        ctx.fillStyle = '#1e293b';
        ctx.fillRect(px, py, cs, cs);
        ctx.strokeStyle = '#334155';
        ctx.lineWidth = 0.5;
        ctx.strokeRect(px + 0.5, py + 0.5, cs - 1, cs - 1);
      }
    }
  }

  // Подсветка клетки под курсором
  const { x, y } = state.hoverCell;
  if (x >= 0 && x < room.width && y >= 0 && y < room.height) {
    const color = state.isDrawing ? '#10b981' : '#3b82f6';
    const alpha = state.isDrawing ? 0.25 : 0.15;
    ctx.strokeStyle = color;
    ctx.lineWidth = 2;
    ctx.strokeRect(x * cs + 1, y * cs + 1, cs - 2, cs - 2);
    ctx.fillStyle = hexToRgba(color, alpha);
    ctx.fillRect(x * cs + 1, y * cs + 1, cs - 2, cs - 2);
  }
}

/** Получить координаты клетки из события */
function getCellFromEvent(e) {
  const rect = canvas.getBoundingClientRect();
  const sx = canvas.width / rect.width;
  const sy = canvas.height / rect.height;
  const mx = (e.clientX - rect.left) * sx;
  const my = (e.clientY - rect.top) * sy;
  const cs = parseInt(canvas.dataset.cellSize) || 30;
  return {
    x: Math.floor(mx / cs),
    y: Math.floor(my / cs),
    cs,
    clientX: e.clientX,
    clientY: e.clientY,
  };
}

// ===== Обработчики мыши =====

function onPointerDown(e) {
  if (state.isBusy) return;
  if (e.button !== 0) return;
  const room = state.currentRoom();
  if (!room) return;

  const cell = getCellFromEvent(e);
  if (!isInsideRoom(room, cell.x, cell.y)) return;

  e.preventDefault();

  state.isDrawing = true;
  state.paintedCellsCount = 0;
  state.lastPaintedCell = { x: -1, y: -1 };
  canvas.classList.add(state.currentTool === 'erase' ? 'erasing' : 'drawing');
  coordHint.classList.add('drawing');

  if (applyToolToCell(room, cell.x, cell.y)) {
    state.lastPaintedCell = { x: cell.x, y: cell.y };
    state.paintedCellsCount++;
    state.markUnsaved();
    updateStats();
  }
  redraw();

  if (canvas.setPointerCapture && e.pointerId != null) {
    try { canvas.setPointerCapture(e.pointerId); } catch {}
  }
}

function onPointerMove(e) {
  const room = state.currentRoom();
  if (!room) return;
  const cell = getCellFromEvent(e);

  // Обновляем hover
  if (isInsideRoom(room, cell.x, cell.y)) {
    state.hoverCell = { x: cell.x, y: cell.y };
    coordHint.textContent = `X: ${cell.x}  Y: ${cell.y}`;
    coordHint.style.display = 'block';
    const rect = canvas.getBoundingClientRect();
    coordHint.style.left = cell.clientX - rect.left + 'px';
    coordHint.style.top = cell.clientY - rect.top + 'px';
    updateCellInfo(cell.x, cell.y, room.cells[cell.y][cell.x] === 1 ? '🧱 Стена' : '⬜ Пол');
  } else {
    state.hoverCell = { x: -1, y: -1 };
    coordHint.style.display = 'none';
    updateCellInfo(null, null, null);
  }

  // Рисование
  if (state.isDrawing) {
    if (state.lastPaintedCell.x === -1) {
      if (isInsideRoom(room, cell.x, cell.y)) {
        if (applyToolToCell(room, cell.x, cell.y)) {
          state.lastPaintedCell = { x: cell.x, y: cell.y };
          state.paintedCellsCount++;
        }
      }
    } else {
      const points = lineBetween(
        state.lastPaintedCell.x,
        state.lastPaintedCell.y,
        cell.x,
        cell.y
      );
      let changed = false;
      for (const p of points) {
        if (applyToolToCell(room, p.x, p.y)) {
          changed = true;
          state.paintedCellsCount++;
        }
      }
      state.lastPaintedCell = { x: cell.x, y: cell.y };
      if (changed) {
        state.markUnsaved();
        updateStats();
      }
    }
  }

  redraw();
}

function onPointerUp(e) {
  if (!state.isDrawing) return;
  state.isDrawing = false;
  canvas.classList.remove('drawing', 'erasing');
  coordHint.classList.remove('drawing');

  if (canvas.releasePointerCapture && e.pointerId != null) {
    try { canvas.releasePointerCapture(e.pointerId); } catch {}
  }

  if (state.paintedCellsCount > 0) {
    renderRoomList();
  }
  state.paintedCellsCount = 0;
  state.lastPaintedCell = { x: -1, y: -1 };

  redraw();
}

function onPointerLeave() {
  if (state.isDrawing) return;
  state.hoverCell = { x: -1, y: -1 };
  coordHint.style.display = 'none';
  updateCellInfo(null, null, null);
  redraw();
}