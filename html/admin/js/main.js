/** Точка входа: инициализация + связка модулей */

import { state } from './state.js';
import { RoomAPI } from './api.js';
import { toast, $ } from './utils.js';

import { initModal, showLogin } from './ui/modal.js';
import {
  initSidebar,
  renderRoomList,
  createRoom,
} from './ui/sidebar.js';
import {
  initInspector,
  syncInspectorFromRoom,
  updateStats,
  updateCellInfo,
} from './ui/inspector.js';
import {
  initToolbar,
  setSaveButtonDirty,
  saveCurrentRoom,
} from './ui/toolbar.js';
import { initCanvas, resizeCanvasToRoom, redraw } from './editor/canvas.js';

// ===== Кнопки/элементы, нужные напрямую =====
const emptyState = $('emptyState');
const canvasSpinner = $('canvasSpinner');
const headerRoomName = $('headerRoomName');
const emptyCreateBtn = $('emptyCreateBtn');

// ===== Инициализация всех модулей =====
function initModules() {
  initModal();
  initSidebar();
  initInspector();
  initToolbar();
  initCanvas();

  emptyCreateBtn.addEventListener('click', createRoom);
}

// ===== Подписки на события состояния =====
function wireEvents() {
  // Выбор комнаты
  state.on('room:select', (id) => {
    const room = state.findRoom(id);
    if (!room) return;
    state.currentRoomId = id;
    emptyState.classList.add('hidden');
    state.markSaved();
    syncInspectorFromRoom();
    resizeCanvasToRoom(room);
    redraw();
    renderRoomList();
  });

  // Нет комнат
  state.on('room:none', () => {
    state.currentRoomId = null;
    emptyState.classList.remove('hidden');
    headerRoomName.textContent = '— комната не выбрана';
    redraw();
  });

  // Комнату переименовали / изменили — обновить список
  state.on('room:renamed', renderRoomList);
  state.on('room:resized', () => {
    syncInspectorFromRoom();
    renderRoomList();
  });
  state.on('room:changed', () => {
    updateStats();
    redraw();
  });
  state.on('room:saved', () => {
    renderRoomList();
  });

  // Индикатор несохранённых изменений
  state.on('unsaved:changed', (dirty) => setSaveButtonDirty(dirty));

  // Busy state — блокируем кнопки + спиннер
  state.on('busy:changed', (busy) => {
    canvasSpinner.classList.toggle('hidden', !busy);
    ['createRoomBtn', 'saveRoomBtn', 'clearRoomBtn', 'fillRoomBtn', 'applySizeBtn', 'logoutBtn']
      .forEach((id) => {
        const el = $(id);
        if (el) el.disabled = busy;
      });
  });

  // Успешная авторизация — загрузить комнаты
  state.on('auth:success', loadRooms);
  state.on('auth:logout', () => {
    renderRoomList();
    emptyState.classList.remove('hidden');
    redraw();
  });

  // Предупреждение при закрытии с несохранёнными изменениями
  window.addEventListener('beforeunload', (e) => {
    if (state.hasUnsavedChanges) {
      e.preventDefault();
      e.returnValue = '';
    }
  });

  // Адаптив
  let resizeTimer;
  window.addEventListener('resize', () => {
    clearTimeout(resizeTimer);
    resizeTimer = setTimeout(() => {
      const room = state.currentRoom();
      if (room) {
        resizeCanvasToRoom(room);
        redraw();
      }
    }, 100);
  });
}

// ===== Загрузка комнат после входа =====
async function loadRooms() {
  state.isBusy = true;
  state.emit('busy:changed', true);
  try {
    state.rooms = await RoomAPI.getRooms();
    if (state.rooms.length) {
      state.emit('room:select', state.rooms[0].id);
    } else {
      emptyState.classList.remove('hidden');
    }
    renderRoomList();
  } catch (e) {
    toast('Ошибка загрузки: ' + e.message, 'error');
  } finally {
    state.isBusy = false;
    state.emit('busy:changed', false);
  }
}

// ===== Старт =====
function start() {
  initModules();
  wireEvents();
  showLogin();
}

start();