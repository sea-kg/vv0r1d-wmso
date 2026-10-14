/** Список комнат */

import { state } from '../state.js';
import { RoomAPI } from '../api.js';
import { escapeHtml, toast, $ } from '../utils.js';

const roomListEl = $('roomList');
const roomCountEl = $('roomCount');
const roomSearchInput = $('roomSearch');
const createRoomBtn = $('createRoomBtn');

export function renderRoomList() {
  const filtered = state.rooms.filter((r) =>
    r.name.toLowerCase().includes(state.searchQuery.toLowerCase())
  );
  roomCountEl.textContent = state.rooms.length;

  if (!filtered.length) {
    roomListEl.innerHTML = `<div style="padding:20px;text-align:center;color:#64748b;font-size:0.8rem;">
      ${state.rooms.length ? 'Ничего не найдено' : 'Нет комнат'}
    </div>`;
    return;
  }

  roomListEl.innerHTML = filtered
    .map(
      (room) => `
    <div class="room-item ${room.id === state.currentRoomId ? 'active' : ''}" data-room-id="${room.id}">
      <div class="icon">🏠</div>
      <div class="info">
        <div class="name">${escapeHtml(room.name)}</div>
        <div class="meta">${room.width}×${room.height} · ${countWalls(room)} стен</div>
      </div>
      <button class="delete-btn" data-delete="${room.id}" title="Удалить">🗑️</button>
    </div>`
    )
    .join('');

  roomListEl.querySelectorAll('.room-item').forEach((el) => {
    el.addEventListener('click', (e) => {
      if (e.target.closest('.delete-btn')) return;
      if (state.isBusy || state.isDrawing) return;
      const id = el.dataset.roomId;
      if (id && id !== state.currentRoomId) state.emit('room:select', id);
    });
  });

  roomListEl.querySelectorAll('.delete-btn').forEach((btn) => {
    btn.addEventListener('click', (e) => {
      e.stopPropagation();
      const id = btn.dataset.delete;
      if (id) deleteRoom(id);
    });
  });
}

export function countWalls(room) {
  let c = 0;
  for (let y = 0; y < room.height; y++) {
    for (let x = 0; x < room.width; x++) {
      if (room.cells[y][x] === 1) c++;
    }
  }
  return c;
}

export async function createRoom() {
  const w = 15, h = 10;
  const cells = Array.from({ length: h }, () => Array(w).fill(0));
  const newRoom = { name: 'Комната ' + (state.rooms.length + 1), width: w, height: h, cells };
  state.isBusy = true;
  state.emit('busy:changed', true);
  try {
    const created = await RoomAPI.createRoom(newRoom);
    state.rooms.push(created);
    state.emit('room:select', created.id);
    renderRoomList();
    toast('Комната создана', 'success');
  } catch (e) {
    toast('Ошибка: ' + e.message, 'error');
  } finally {
    state.isBusy = false;
    state.emit('busy:changed', false);
  }
}

export async function deleteRoom(id) {
  const room = state.findRoom(id);
  if (!room) return;
  if (!confirm(`Удалить комнату «${room.name}»?`)) return;
  state.isBusy = true;
  state.emit('busy:changed', true);
  try {
    await RoomAPI.deleteRoom(id);
    state.rooms = state.rooms.filter((r) => r.id !== id);
    if (state.currentRoomId === id) {
      state.currentRoomId = null;
      if (state.rooms.length) {
        state.emit('room:select', state.rooms[0].id);
      } else {
        state.emit('room:none');
      }
    }
    renderRoomList();
    toast('Комната удалена', 'info');
  } catch (e) {
    toast('Ошибка: ' + e.message, 'error');
  } finally {
    state.isBusy = false;
    state.emit('busy:changed', false);
  }
}

export function initSidebar() {
  createRoomBtn.addEventListener('click', createRoom);
  roomSearchInput.addEventListener('input', (e) => {
    state.searchQuery = e.target.value;
    renderRoomList();
  });
}