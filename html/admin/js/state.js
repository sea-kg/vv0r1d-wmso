/**
 * Глобальное состояние + простая событийная шина.
 * Любой модуль может подписаться на изменения: state.on('room:changed', fn)
 */

const listeners = new Map();

export const state = {
  rooms: [],
  currentRoomId: null,
  currentTool: 'wall',
  isBusy: false,
  searchQuery: '',
  hasUnsavedChanges: false,

  // Drag-состояние
  isDrawing: false,
  lastPaintedCell: { x: -1, y: -1 },
  paintedCellsCount: 0,
  hoverCell: { x: -1, y: -1 },

  // ----- Методы -----
  findRoom(id) {
    return this.rooms.find((r) => r.id === id) || null;
  },

  currentRoom() {
    return this.findRoom(this.currentRoomId);
  },

  markUnsaved() {
    this.hasUnsavedChanges = true;
    this.emit('unsaved:changed', true);
  },

  markSaved() {
    this.hasUnsavedChanges = false;
    this.emit('unsaved:changed', false);
  },

  // ----- События -----
  on(event, fn) {
    if (!listeners.has(event)) listeners.set(event, new Set());
    listeners.get(event).add(fn);
    return () => listeners.get(event).delete(fn);
  },

  emit(event, payload) {
    if (!listeners.has(event)) return;
    listeners.get(event).forEach((fn) => {
      try {
        fn(payload);
      } catch (e) {
        console.error(`[state] listener error for "${event}":`, e);
      }
    });
  },
};