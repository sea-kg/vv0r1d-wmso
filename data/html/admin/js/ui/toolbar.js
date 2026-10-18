/** Инструменты + кнопки очистить/заполнить/сохранить */

import { state } from '../state.js';
import { RoomAPI } from '../api.js';
import { toast, $ } from '../utils.js';

const toolButtons = document.querySelectorAll('.tool-btn');
const clearRoomBtn = $('clearRoomBtn');
const fillRoomBtn = $('fillRoomBtn');
const saveRoomBtn = $('saveRoomBtn');

export function setTool(tool) {
  state.currentTool = tool;
  toolButtons.forEach((b) => b.classList.toggle('active', b.dataset.tool === tool));
}

export function clearRoom() {
  const room = state.currentRoom();
  if (!room) return;
  for (let y = 0; y < room.height; y++) {
    for (let x = 0; x < room.width; x++) room.cells[y][x] = 0;
  }
  state.markUnsaved();
  state.emit('room:changed');
  toast('Сетка очищена (не забудьте сохранить)', 'info');
}

export function fillRoom() {
  const room = state.currentRoom();
  if (!room) return;
  for (let y = 0; y < room.height; y++) {
    for (let x = 0; x < room.width; x++) room.cells[y][x] = 1;
  }
  state.markUnsaved();
  state.emit('room:changed');
  toast('Всё заполнено стенами', 'info');
}

export async function saveCurrentRoom() {
  const room = state.currentRoom();
  if (!room) return;
  const updated = {
    name: room.name,
    width: room.width,
    height: room.height,
    cells: room.cells,
  };
  state.isBusy = true;
  state.emit('busy:changed', true);
  try {
    const saved = await RoomAPI.updateRoom(room.id, updated);
    Object.assign(room, saved);
    state.markSaved();
    state.emit('room:saved');
    toast('Сохранено', 'success');
  } catch (e) {
    toast('Ошибка: ' + e.message, 'error');
  } finally {
    state.isBusy = false;
    state.emit('busy:changed', false);
  }
}

export function setSaveButtonDirty(dirty) {
  saveRoomBtn.textContent = dirty ? '💾 Сохранить *' : '💾 Сохранить';
}

export function initToolbar() {
  toolButtons.forEach((btn) => {
    btn.addEventListener('click', () => setTool(btn.dataset.tool));
  });
  clearRoomBtn.addEventListener('click', clearRoom);
  fillRoomBtn.addEventListener('click', fillRoom);
  saveRoomBtn.addEventListener('click', saveCurrentRoom);

  // Горячие клавиши
  document.addEventListener('keydown', (e) => {
    if ((e.ctrlKey || e.metaKey) && e.key === 's') {
      e.preventDefault();
      if (state.currentRoomId && !state.isBusy) saveCurrentRoom();
      return;
    }
    if (e.target.tagName === 'INPUT') return;
    if (e.key === '1') setTool('wall');
    if (e.key === '2') setTool('floor');
    if (e.key === '3') setTool('erase');
  });
}