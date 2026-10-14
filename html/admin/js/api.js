/**
 * API-слой. Для подключения реального бэкенда:
 * 1. Установите API_CONFIG.useMock = false
 * 2. Укажите API_CONFIG.baseUrl
 * 3. Раскомментируйте реальную реализацию ниже и удалите мок.
 */

export const API_CONFIG = {
  baseUrl: '/api',
  useMock: true,
};

// ===== РЕАЛЬНАЯ РЕАЛИЗАЦИЯ (закомментирована) =====
/*
export const RoomAPI = {
  async login(username, password) {
    const r = await fetch(`${API_CONFIG.baseUrl}/auth/login`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ username, password }),
    });
    if (!r.ok) throw new Error('Неверные учётные данные');
    const data = await r.json();
    localStorage.setItem('admin_token', data.token);
    return data;
  },
  async logout() {
    const t = localStorage.getItem('admin_token');
    await fetch(`${API_CONFIG.baseUrl}/auth/logout`, {
      method: 'POST',
      headers: { Authorization: `Bearer ${t}` },
    }).catch(() => {});
    localStorage.removeItem('admin_token');
  },
  async getRooms() {
    const r = await fetch(`${API_CONFIG.baseUrl}/rooms`, {
      headers: { Authorization: `Bearer ${localStorage.getItem('admin_token')}` },
    });
    if (!r.ok) throw new Error('Ошибка загрузки');
    return r.json();
  },
  async createRoom(room) {
    const r = await fetch(`${API_CONFIG.baseUrl}/rooms`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        Authorization: `Bearer ${localStorage.getItem('admin_token')}`,
      },
      body: JSON.stringify(room),
    });
    if (!r.ok) throw new Error('Ошибка создания');
    return r.json();
  },
  async updateRoom(id, room) {
    const r = await fetch(`${API_CONFIG.baseUrl}/rooms/${id}`, {
      method: 'PUT',
      headers: {
        'Content-Type': 'application/json',
        Authorization: `Bearer ${localStorage.getItem('admin_token')}`,
      },
      body: JSON.stringify(room),
    });
    if (!r.ok) throw new Error('Ошибка сохранения');
    return r.json();
  },
  async deleteRoom(id) {
    const r = await fetch(`${API_CONFIG.baseUrl}/rooms/${id}`, {
      method: 'DELETE',
      headers: { Authorization: `Bearer ${localStorage.getItem('admin_token')}` },
    });
    if (!r.ok) throw new Error('Ошибка удаления');
  },
};
*/

const STORAGE_KEY = 'game_rooms';
const delay = (ms) => new Promise((r) => setTimeout(r, ms));

const MockStorage = {
  _rooms: null,
  _load() {
    if (this._rooms) return this._rooms;
    try {
      const s = localStorage.getItem(STORAGE_KEY);
      this._rooms = s ? JSON.parse(s) : null;
    } catch {
      this._rooms = null;
    }
    return this._rooms;
  },
  _save() {
    try {
      localStorage.setItem(STORAGE_KEY, JSON.stringify(this._rooms));
    } catch {}
  },
  _defaults() {
    const make = (name, w, h) => {
      const cells = Array.from({ length: h }, () => Array(w).fill(0));
      for (let x = 0; x < w; x++) {
        cells[0][x] = 1;
        cells[h - 1][x] = 1;
      }
      for (let y = 0; y < h; y++) {
        cells[y][0] = 1;
        cells[y][w - 1] = 1;
      }
      return {
        id: 'room_' + Math.random().toString(36).slice(2, 9),
        name,
        width: w,
        height: h,
        cells,
      };
    };
    return [
      make('Гостиная', 12, 8),
      make('Кухня', 10, 6),
      make('Подземелье', 20, 15),
    ];
  },
  get() {
    let r = this._load();
    if (!r) {
      r = this._defaults();
      this._rooms = r;
      this._save();
    }
    return JSON.parse(JSON.stringify(r));
  },
  set(r) {
    this._rooms = JSON.parse(JSON.stringify(r));
    this._save();
  },
};

export const RoomAPI = {
  async login(u, p) {
    await delay(350);
    if (u === 'admin' && p === 'admin123') {
      localStorage.setItem('mock_token', 'mock_' + Date.now());
      return { token: 'mock' };
    }
    throw new Error('Неверный логин или пароль');
  },
  async logout() {
    await delay(120);
    localStorage.removeItem('mock_token');
  },
  async getRooms() {
    await delay(250);
    return MockStorage.get();
  },
  async createRoom(room) {
    await delay(250);
    const rooms = MockStorage.get();
    const newRoom = { ...room, id: 'room_' + Math.random().toString(36).slice(2, 9) };
    rooms.push(newRoom);
    MockStorage.set(rooms);
    return newRoom;
  },
  async updateRoom(id, room) {
    await delay(250);
    const rooms = MockStorage.get();
    const i = rooms.findIndex((r) => r.id === id);
    if (i === -1) throw new Error('Комната не найдена');
    rooms[i] = { ...rooms[i], ...room, id };
    MockStorage.set(rooms);
    return rooms[i];
  },
  async deleteRoom(id) {
    await delay(200);
    MockStorage.set(MockStorage.get().filter((r) => r.id !== id));
  },
};