/** Панель параметров: название, размеры, статистика, координаты */

import { state } from '../state.js';
import { clamp, $ } from '../utils.js';

const roomNameInput = $('roomNameInput');
const roomWidthInput = $('roomWidthInput');
const roomHeightInput = $('roomHeightInput');
const applySizeBtn = $('applySizeBtn');
const presetChips = $('presetChips');
const headerRoomName = $('headerRoomName');
const statTotal = $('statTotal');
const statWalls = $('statWalls');
const statFloor = $('statFloor');
const statPercent = $('statPercent');
const cellX = $('cellX');
const cellY = $('cellY');
const cellType = $('cellType');

/** Заполнить поля из текущей комнаты */
export function syncInspectorFromRoom() {
  const room = state.currentRoom();
  if (!room) return;
  roomNameInput.value = room.name;
  roomWidthInput.value = room.width;
  roomHeightInput.value = room.height;
  headerRoomName.textContent = `${room.name} · ${room.width}×${room.height}`;
  updateStats();
}

/** Обновить статистику */
export function updateStats() {
  const room = state.currentRoom();
  if (!room) return;
  const total = room.width * room.height;
  let walls = 0;
  for (let y = 0; y < room.height; y++) {
    for (let x = 0; x < room.width; x++) {
      if (room.cells[y][x] === 1) walls++;
    }
  }
  statTotal.textContent = total;
  statWalls.textContent = walls;
  statFloor.textContent = total - walls;
  statPercent.textContent = Math.round((walls / total) * 100) + '%';
}

/** Применить новые размеры (пересоздать сетку с сохранением) */
export function applyRoomSize() {
  const room = state.currentRoom();
  if (!room) return;
  const newW = clamp(parseInt(roomWidthInput.value) || room.width, 3, 100);
  const newH = clamp(parseInt(roomHeightInput.value) || room.height, 3, 100);
  roomWidthInput.value = newW;
  roomHeightInput.value = newH;

  if (newW === room.width && newH === room.height) return;

  const newCells = [];
  for (let y = 0; y < newH; y++) {
    const row = [];
    for (let x = 0; x < newW; x++) {
      row.push(y < room.height && x < room.width ? room.cells[y][x] : 0);
    }
    newCells.push(row);
  }
  room.cells = newCells;
  room.width = newW;
  room.height = newH;

  headerRoomName.textContent = `${room.name} · ${room.width}×${room.height}`;
  state.markUnsaved();
  state.emit('room:resized');
  state.emit('room:changed');
}

/** Обновить координаты клетки под курсором */
export function updateCellInfo(x, y, type) {
  cellX.value = x === null ? '—' : x;
  cellY.value = y === null ? '—' : y;
  cellType.value = type || '—';
}

export function initInspector() {
  applySizeBtn.addEventListener('click', applyRoomSize);

  [roomWidthInput, roomHeightInput].forEach((inp) => {
    inp.addEventListener('keypress', (e) => {
      if (e.key === 'Enter') applyRoomSize();
    });
  });

  presetChips.querySelectorAll('.chip').forEach((chip) => {
    chip.addEventListener('click', () => {
      presetChips.querySelectorAll('.chip').forEach((c) => c.classList.remove('active'));
      chip.classList.add('active');
      roomWidthInput.value = chip.dataset.w;
      roomHeightInput.value = chip.dataset.h;
    });
  });

  // Имя комнаты — помечаем unsaved при изменении
  roomNameInput.addEventListener('input', () => {
    const room = state.currentRoom();
    if (!room) return;
    room.name = roomNameInput.value.trim() || 'Без названия';
    headerRoomName.textContent = `${room.name} · ${room.width}×${room.height}`;
    state.markUnsaved();
    state.emit('room:renamed');
  });
}